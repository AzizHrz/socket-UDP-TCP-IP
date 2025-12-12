# TCP Fork-based Multiclient Monoserveur

This project demonstrates a TCP server that handles multiple clients concurrently using fork() for process-based concurrency, providing directory listing service.

## Communication Type
- **Protocol**: TCP
- **Type**: Connection-oriented, reliable communication
- **Concurrency**: Process-based (fork) for handling multiple clients
- **Service**: Single service (directory listing with tools)

## Socket Programming Details

### Server Side (`serveurTCP_fork.c`)
1. **Socket Creation**: `socket(AF_INET, SOCK_STREAM, 0)` creates listening socket
2. **Address Binding**: `bind()` associates socket with server address and port
3. **Listen Mode**: `listen()` enables socket to accept multiple connections
4. **Concurrent Processing**: Uses `fork()` to create child process for each client
5. **Connection Handling**: Parent continues accepting connections, children handle individual clients
6. **Service Implementation**: Child processes use `tools.c` functions for directory operations
7. **Process Management**: Children exit after serving client, parent manages connection queue

### Client Side (`clientTCP_fork.c`)
1. **Socket Creation**: `socket(AF_INET, SOCK_STREAM, 0)` creates TCP socket
2. **Server Connection**: `connect()` establishes connection to server
3. **Interactive Communication**: Sends commands and receives responses in a loop
4. **Data Exchange**: Uses `write()` for sending commands, `read()` for receiving responses
5. **Connection Persistence**: Keeps connection open for multiple interactions

### Communication Flow
1. Server binds to port and listens for connections
2. Multiple clients can connect simultaneously
3. For each client connection, server forks a child process
4. Child process handles all communication with that specific client
5. Clients send commands (e.g., directory listing requests)
6. Server processes requests using utility functions
7. Responses sent back over TCP connections
8. Child processes terminate after client disconnects
9. Parent server continues accepting new connections

### Key Socket Functions
- `fork()`: Creates process concurrency for multi-client handling
- `signal(SIGCHLD, SIG_IGN)`: Prevents zombie processes
- `tools.c`: Provides `send_msg()` for robust message sending and directory utilities

## Files
- `clientTCP_fork.c`: TCP client implementation
- `serveurTCP_fork.c`: TCP server using fork for concurrency
- `tools.c`, `tools.h`: Utility functions for directory operations
- `Makefile`: Build configuration

## How to Run

1. **Build the project**:
   ```bash
   make
   ```

2. **Start the server** (in one terminal):
   ```bash
   ./serveurTCP_fork
   ```

3. **Run clients** (in multiple terminals):
   ```bash
   ./clientTCP_fork
   ```

4. **Interaction**: Multiple clients can connect simultaneously, request directory listings, and the server handles each in a separate process.

5. **Clean up**:
   ```bash
   make clean
   ```