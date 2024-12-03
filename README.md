## Client-Server Communication in C

This project demonstrates a simple client-server communication model in C using socket programming. The client and server communicate over a fixed port (30041) and are capable of back-and-forth messaging. The program also employs multi-threading to handle asynchronous read and write operations.

## Features

- **Socket Programming:** Implements TCP communication between client and server.
- **Multi-threading:** Allows simultaneous reading from and writing to sockets without blocking.
- **Custom Makefile:** Simplifies compilation and cleanup processes.
- **Fixed Port:** Communication is established on port `30041`.
## Prerequisites

- GCC compiler
- Make utility
- Linux or Unix-based environment
- PuTTY (or similar terminal tool) for testing multiple instances

---

## How to Compile
Compile the programs using the Makefile:
make
./server
./client
2 terminals, communicate back and forth
