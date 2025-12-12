# Client-Server UDP

This project demonstrates basic UDP (User Datagram Protocol) client-server communication in C.

## Communication Type
- **Protocol**: UDP
- **Type**: Connectionless, unreliable, datagram-based communication
- **Features**: No connection establishment, faster but no guaranteed delivery or order

## Socket Programming Details

### Server Side (`serveurUDP.c`)
1. **Socket Creation**: `socket(AF_INET, SOCK_DGRAM, 0)` creates a UDP socket
2. **Address Configuration**: Sets up `struct sockaddr_in` with IP address and port
3. **Binding**: `bind()` associates the socket with the server address
4. **Receiving Data**: `recvfrom()` blocks to receive datagrams from clients, captures sender address
5. **Sending Data**: `sendto()` sends datagrams back to the client using captured address
6. **No Connection State**: Each datagram is independent, no persistent connection

### Client Side (`clientUDP.c`)
1. **Socket Creation**: `socket(AF_INET, SOCK_DGRAM, 0)` creates a UDP socket
2. **Server Address Setup**: Configures `struct sockaddr_in` with server's IP and port
3. **Sending Data**: `sendto()` sends datagrams to the server
4. **Receiving Data**: `recvfrom()` receives response datagrams from server
5. **No Connection Establishment**: Direct datagram exchange without handshake

### Communication Flow
1. Server starts, binds to port, and waits for datagrams
2. Client creates socket and sends datagram to server address
3. Server receives datagram, processes it, and sends response back to client's address
4. Each exchange is independent - no persistent connection
5. UDP provides best-effort delivery with no reliability guarantees

## Files
- `clientUDP.c`: UDP client implementation
- `serveurUDP.c`: UDP server implementation
- `Makefile`: Build configuration

## How to Run

1. **Build the project**:
   ```bash
   make
   ```

2. **Start the server** (in one terminal):
   ```bash
   ./serveurUDP
   ```

3. **Run the client** (in another terminal):
   ```bash
   ./clientUDP
   ```

4. **Interaction**: The client sends datagrams to the server, exchanges messages, and demonstrates basic UDP communication.

5. **Clean up**:
   ```bash
   make clean
   ```