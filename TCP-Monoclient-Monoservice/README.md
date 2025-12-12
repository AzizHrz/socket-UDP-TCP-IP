# TCP Monoclient Monoservice

This project demonstrates a TCP client-server setup with a single client connecting to a server that provides directory listing service.

## Communication Type
- **Protocol**: TCP
- **Type**: Connection-oriented, reliable communication
- **Architecture**: Single client, single service (directory listing)

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