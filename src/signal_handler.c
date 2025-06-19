#include <signal_handler.h>

void handler() {
  /* TODO: join() all threads before shutting down */
  printf("Gracefully shutting down. Bye!\n");
  fflush(stdout);
  exit(0);
}

int create_handlers() {
    struct sigaction sa = {0};
    sa.sa_handler = handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;

    sigaction(SIGINT, &sa, NULL);   /* Ctrl+C */
    sigaction(SIGTERM, &sa, NULL);  /* kill <pid> */
    sigaction(SIGQUIT, &sa, NULL);  /* Ctrl+\ */
    sigaction(SIGHUP, &sa, NULL);   /* terminal closed */
    sigaction(SIGPIPE, &sa, NULL);  /* write to closed socket */

    return 0;
}
