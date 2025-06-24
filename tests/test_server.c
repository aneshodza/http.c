#include <CUnit/CUnit.h>
#include <server.h>
#include <signal.h>

void test_general_requests() {
  int pipefd[2];
  pipe(pipefd);

  int pid = fork();

  if (pid == 0) {

    /* ===== CHILD: Starts the server ===== */

    close(pipefd[0]);
    /* Mutes the console output of the server */
    freopen("/dev/null", "w", stdout);
    freopen("/dev/null", "w", stderr);

    int server_fd;
    struct sockaddr_in address;
    int port = spawn_server(&server_fd, &address);

    /* Writes the port the server started on into a pipe */
    if (port == -1) {
      write(pipefd[1], "-1", 2);
    }
    dprintf(pipefd[1], "%d", port);
    close(pipefd[1]);

    create_threadpool();
    spawn_listener(&server_fd, &address);
  } else {

    /* ===== PARENT: Requests the server ===== */

    close(pipefd[1]);
    char buf[16] = {0};
    read(pipefd[0], buf, sizeof(buf) - 1);
    close(pipefd[0]);

    int port = atoi(buf);
    CU_ASSERT_NOT_EQUAL(port, -1);

    int retries = 0;
    while (retries < 20 && retries != -1) {
      char cmd[256];
      snprintf(cmd, sizeof(cmd),
               "curl -s --head --fail -H \"Accept: text/html\" http://localhost:%d/ > /dev/null",
               port);
      int res = system(cmd);
      if (res == 0) {
        /* If curl worked exit the setup */
        retries = -1;
      } else {
        /* Otherwise wait 200ms */
        retries++;
        usleep(200000);
      }
    }
    if (retries == 20) CU_FAIL("Server could not start");

    CU_ASSERT_EQUAL(retries, -1);
    kill(pid, 9);
  }
}

void setup_test_server(CU_pSuite *suite) {
  CU_add_test(*suite, "Starts the server properly and accepts requests", test_general_requests);
}
