# TCP Monoclient Monoservice

This project demonstrates a TCP client-server setup with a single client connecting to a server that provides directory listing service.

## Communication Type
- **Protocol**: TCP
- **Type**: Connection-oriented, reliable communication
- **Architecture**: Single client, single service (directory listing)

## Socket Programming Details

### Server Side (`serveurTCP.c`)
1. **Socket Creation**: `socket(AF_INET, SOCK_STREAM, 0)` creates a TCP socket for listening
2. **Address Binding**: `bind()` associates socket with local address and port
3. **Listen Mode**: `listen()` prepares socket to accept incoming connections
4. **Accept Connection**: `accept()` blocks until client connects, returns connected socket
5. **Service Provision**: Reads client requests and provides directory listing using `opendir()`/`readdir()`
6. **Data Transmission**: Uses `write()` to send directory contents to client
7. **Connection Management**: Handles one client at a time, closes connection after service

### Client Side (`clientTCP.c`)
1. **Socket Creation**: `socket(AF_INET, SOCK_STREAM, 0)` creates a TCP socket
2. **Server Connection**: `connect()` establishes connection to server's IP and port
3. **Request Sending**: Sends service request to server
4. **Response Reception**: Uses `read()` to receive directory listing from server
5. **Data Display**: Prints received directory contents to stdout

### Communication Flow
1. Server binds to port and listens for connections
2. Client connects to server using IP address and port
3. Client sends request for directory listing
4. Server processes request, reads directory contents
5. Server sends directory listing back to client over TCP stream
6. Client displays received data
7. Connection closes after single request-response cycle

## Files
- `clientTCP.c`: TCP client that connects and requests directory listing
- `serveurTCP.c`: TCP server that provides directory listing service
- `Makefile`: Build configuration
- `Readme.md`: Original project description

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

4. **Interaction**: The client connects to the server, requests directory contents, and the server responds with the listing.

5. **Clean up**:
   ```bash
   make clean
   ```