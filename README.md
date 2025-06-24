[![Build](https://github.com/aneshodza/http.c/actions/workflows/ci.yml/badge.svg)](https://github.com/aneshodza/http.c/actions)

# http.c – A Multithreaded HTTP Server in C

Welcome to **http.c**, a lightweight and multithreaded HTTP server written in pure C by Anes Hodza. It supports basic HTTP request handling, MIME type resolution, error pages, and a simple thread pool for concurrent request processing.

```
    __    __  __             
   / /_  / /_/ /_____   _____
  / __ \/ __/ __/ __ \ / ___/
 / / / / /_/ /_/ /_/ // /__  
/_/ /_/\__/\__/ .___(_)___/  
             /_/             
```

## 🚀 Features

- Supports HTTP/1.1 GET requests
- Multithreaded using a custom thread pool
- MIME type detection for served files
- Custom error pages (e.g., 404, 500) with fallback defaults
- Cleanly separated source and header structure
- Unit tests using [CUnit](https://cunit.sourceforge.net/)
- Dynamic port binding with fallback if a port is in use

## 🧱 Project Structure

```
.
├── include/         # Header files
├── public/          # Static files served by the server
│   └── error_pages/ # Custom error pages (e.g. 404.html, 500.html)
├── src/             # Main source code
├── tests/           # Unit tests
├── Makefile
└── README.md
```

## 🛠 Build Instructions

### 🔧 Prerequisites

- `gcc`
- [`CUnit`](https://formulae.brew.sh/formula/cunit) (e.g., install via Homebrew: `brew install cunit`)

### 🧪 Build and Run Tests

```sh
make test
```

### 🔨 Build the Server

```sh
make
```

### ▶️ Run the Server

```sh
./out/executable
```

> On startup, the server will print the port it's bound to. You can then access it via your browser or tools like `curl`.

## 🧪 Example

```sh
curl -H "Accept: text/html" http://localhost:8080/index.html
```

If the file exists in `public/`, it will be served with the appropriate MIME type. Otherwise, you'll receive a custom or default error response.

## ⚠️ Error Handling

- Custom error pages can be placed under: `public/error_pages/`
  - Examples:
    - `404.html`
    - `500.html`
- If a custom error page is missing, a fallback hardcoded HTML response is sent.
- To make a custom error page you simply create a file of following format: `<HTTP-ERROR-CODE>.html`.
- Custom error pages always are `.html` Files (for now).

## 🧼 Clean Build Artifacts

```sh
make clean
```

## 🧪 Writing Tests

- Tests are written using the CUnit framework in `tests/`.
- Most tests are unit tests, where a specific function is tested with input -> output.
- The server in its entirety is tested in `tests/test_server.c`, where the server is started in a child process. The parent then probes if the server is up using `curl`.

---

## 🆗 Accepted request header

The server only supports certain request types.  
Unsupported request types get a `405 - Method not allowed`.
- GET
- HEAD

## 👨‍💻 Author

This project was written by **Anes Hodza**  
GitHub: [@aneshodza](https://github.com/aneshodza)
