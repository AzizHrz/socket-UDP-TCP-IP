# TCP Fork-based Multiclient Monoserveur

This project demonstrates a TCP server that handles multiple clients concurrently using fork() for process-based concurrency, providing directory listing service.

## Communication Type
- **Protocol**: TCP
- **Type**: Connection-oriented, reliable communication
- **Concurrency**: Process-based (fork) for handling multiple clients
- **Service**: Single service (directory listing with tools)

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