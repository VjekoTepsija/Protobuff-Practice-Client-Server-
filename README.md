# Protobuff-Practice-Client-Server-
Simple protobuff project which I used to play around(and lose my nerves) with protobuff and google test(and CMake, that caused the most pain).
# Proto2 C++ Client–Server Demo

Aside C++ I used:
- **Protocol Buffers (Proto2)**
- **CMake**
- **GoogleTest**

## Project Overview

The project consists of three parts:

### Client
- Collects user input (string or arithmetic task)
- Creates a request.bin with the data for the task
- Launches the server
- Reads the response.bin from the server

### Server
- Reads the request
- Processes the request based on task type
- Puts the result/error into a response.bin

### Protocol
- All communication is defined in `main_proto.proto`
- Multiple error codes for invalid requests
cmake --build out/build

