# Client-Server TCP

This project demonstrates basic TCP (Transmission Control Protocol) client-server communication in C.

## Communication Type
- **Protocol**: TCP
- **Type**: Connection-oriented, reliable, stream-based communication
- **Features**: Establishes a connection before data exchange, ensures data delivery and order

## Files
- `clientTCP.c`: TCP client implementation
- `serveurTCP.c`: TCP server implementation
- `Makefile`: Build configuration

## How to Run

1. **Build the project**:
   ```bash
   make
   ```

2. **Start the server** (in one terminal):
   ```bash
   ./serveurTCP
   ```

3. **Run the client** (in another terminal):
   ```bash
   ./clientTCP
   ```

4. **Interaction**: The client connects to the server, exchanges messages, and demonstrates basic TCP communication.

5. **Clean up**:
   ```bash
   make clean
   ```