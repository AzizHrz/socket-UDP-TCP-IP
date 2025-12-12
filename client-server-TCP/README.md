# Client-Server TCP

This project demonstrates basic TCP (Transmission Control Protocol) client-server communication in C.

## Communication Type
- **Protocol**: TCP
- **Type**: Connection-oriented, reliable, stream-based communication
- **Features**: Establishes a connection before data exchange, ensures data delivery and order

## Socket Programming Details

### Server Side (`serveurTCP.c`)
1. **Socket Creation**: `socket(AF_INET, SOCK_STREAM, 0)` creates a TCP socket
2. **Address Configuration**: Sets up `struct sockaddr_in` with IP address and port
3. **Binding**: `bind()` associates the socket with the server address
4. **Listening**: `listen()` puts the socket in passive mode to accept connections
5. **Accepting Connections**: `accept()` blocks until a client connects, returns new socket for communication
6. **Data Exchange**: Uses `read()` and `write()` for bidirectional communication
7. **Connection Closure**: `close()` terminates the connection

### Client Side (`clientTCP.c`)
1. **Socket Creation**: `socket(AF_INET, SOCK_STREAM, 0)` creates a TCP socket
2. **Server Address Setup**: Configures `struct sockaddr_in` with server's IP and port
3. **Connection Establishment**: `connect()` initiates three-way handshake with server
4. **Data Exchange**: Uses `read()` and `write()` for communication with server
5. **Connection Closure**: `close()` ends the connection

### Communication Flow
1. Server starts, binds to port, and listens for connections
2. Client creates socket and connects to server
3. Once connected, both can exchange data using stream I/O
4. Connection remains open until explicitly closed
5. TCP ensures reliable, ordered delivery of data

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