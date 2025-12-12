# Client-Server UDP

This project demonstrates basic UDP (User Datagram Protocol) client-server communication in C.

## Communication Type
- **Protocol**: UDP
- **Type**: Connectionless, unreliable, datagram-based communication
- **Features**: No connection establishment, faster but no guaranteed delivery or order

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