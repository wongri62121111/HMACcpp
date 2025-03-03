# HMAC Socket Communication Project

This project demonstrates a basic implementation of HMAC (Hash-based Message Authentication Code) payload communication between a server and a client using networking sockets in C++. The project uses the `hmac_sha256` library for HMAC computation and includes a simple benchmarking mechanism to measure the time taken for sending, receiving, reading, and writing HMAC payloads.

---

## Project Structure

The project consists of the following files:

```
hmac_socket_project/
│
├── hmac_sha256.h (from https://github.com/h5p9sl/hmac_sha256)
├── server.cpp
├── client.cpp
└── README.md
```

---

## Dependencies

1. **Windows OS**: This project is designed to run on Windows.
2. **g++**: The GNU C++ compiler is required to compile the code.
3. **Winsock**: The Windows Sockets API is used for networking.

---

## Installation and Setup

### Step 1: Install g++
If you don't already have `g++` installed, you can install it via [MinGW](http://www.mingw.org/) or [MSYS2](https://www.msys2.org/). Ensure `g++` is added to your system's PATH.

### Step 2: Download the `hmac_sha256.h` File
Download the `hmac_sha256.h` file from the repository: [h5p9sl/hmac_sha256](https://github.com/h5p9sl/hmac_sha256) and place it in the project folder.

### Step 3: Open the Project in Visual Studio Code
1. Open Visual Studio Code.
2. Open the folder containing the project files (`server.cpp`, `client.cpp`, and `hmac_sha256.h`).

---

## Compilation

To compile the server and client programs, open a terminal in Visual Studio Code and run the following commands:

```bash
g++ server.cpp -o server.exe -lws2_32
g++ client.cpp -o client.exe -lws2_32
```

This will generate two executables: `server.exe` and `client.exe`.

---

## Running the Program

### Step 1: Start the Server
Run the server executable in a terminal:

```bash
./server.exe
```

The server will start listening on port `8080` and wait for incoming connections.

### Step 2: Start the Client
In a separate terminal, run the client executable:

```bash
./client.exe
```

The client will connect to the server, send an HMAC payload, and receive a response.

---

## Benchmarking

The program uses the `chrono` library to measure the time taken for:
1. Sending the HMAC payload from the client to the server.
2. Receiving the HMAC payload on the server.
3. Sending the response from the server to the client.
4. Receiving the response on the client.

The benchmark results are printed to the console.

---

## Example Output

### Server Output
```
Server listening on port 8080...
Received HMAC payload: Hello, server!
Time to receive: 0.000123 seconds
```

### Client Output
```
Time to send: 0.000456 seconds
Server response: HMAC verified!
```

---

## Customization

1. **Change the HMAC Key**: Modify the `key` variable in both `server.cpp` and `client.cpp`.
2. **Change the Message**: Modify the `message` variable in `client.cpp`.
3. **Change the Port**: Update the `PORT` macro in both files if needed.

---

## Notes

- This implementation is minimal and designed for educational purposes.
- Error handling is basic and can be expanded for production use.
- The project assumes the server and client are running on the same machine (`localhost`). To test across different machines, update the IP address in `client.cpp`.

---

## Troubleshooting

1. **Winsock Errors**: Ensure `ws2_32.lib` is linked during compilation.
2. **Port Conflicts**: Ensure no other application is using port `8080`.
3. **Compiler Issues**: Verify that `g++` is installed and configured correctly.