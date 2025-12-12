# TCP Monoclient Monoservice

This project demonstrates a TCP client-server setup with a single client connecting to a server that provides directory listing service.

## Communication Type
- **Protocol**: TCP
- **Type**: Connection-oriented, reliable communication
- **Architecture**: Single client, single service (directory listing)
- **Authentication**: Username/password required before service access

## Socket Programming Details

### Server Side (`serveurTCP.c`)
1. **Socket Creation**: `socket(AF_INET, SOCK_STREAM, 0)` creates a TCP socket for listening
2. **Address Binding**: `bind()` associates socket with local address and port
3. **Listen Mode**: `listen()` prepares socket to accept incoming connections
4. **Accept Connection**: `accept()` blocks until client connects, returns connected socket
5. **Authentication**: Prompts for username/password, validates credentials before allowing access
6. **Service Provision**: Reads client requests and provides directory listing using `opendir()`/`readdir()`
7. **Data Transmission**: Uses `write()` to send directory contents to client
8. **Connection Management**: Handles one client at a time, closes connection after service

### Client Side (`clientTCP.c`)
1. **Socket Creation**: `socket(AF_INET, SOCK_STREAM, 0)` creates a TCP socket
2. **Server Connection**: `connect()` establishes connection to server's IP and port
3. **Authentication**: Responds to username/password prompts from server
4. **Interactive Communication**: Sends commands and receives responses in a loop
5. **Data Exchange**: Uses `write()` for sending commands, `read()` for receiving responses
6. **Connection Persistence**: Keeps connection open for multiple interactions

### Communication Flow
1. Server binds to port and listens for connections
2. Client connects to server using IP address and port
3. Server prompts for username and password
4. Client provides credentials
5. Server validates and grants/denies access
6. If authenticated, client can send service requests
7. Server processes requests, sends responses over TCP stream
8. Connection closes when client quits

### Authentication Details
- **Username**: admin
- **Password**: pass
- Authentication must succeed before any service commands are accepted

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