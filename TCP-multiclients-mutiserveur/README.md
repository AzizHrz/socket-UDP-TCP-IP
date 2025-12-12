# TCP Multiclients Multiservice

This project demonstrates an advanced multiservice TCP server handling multiple clients with multiple specialized services using threads.

## Communication Type
- **Protocol**: TCP
- **Type**: Connection-oriented, reliable communication
- **Concurrency**: Thread-based for handling multiple clients
- **Services**: Multiple services (date/time, directory listing, file content, uptime)

## Services and Ports
- Main Server: Port 9000
- Date/Time Service: Port 9001
- Directory Listing Service: Port 9002
- File Content Service: Port 9003
- Uptime Service: Port 9004

## Files
- `main_server.c`: Main server handling client connections and routing to services
- `client.c`: Client for interacting with the multiservice server
- Individual service servers: `datetime_server.c`, `lists_dir_server.c`, `cat_dir_server.c`, `elapsed_server.c`
- `network.h`, `ports.h`: Header files with definitions
- `Makefile`: Build configuration
- `run_all.sh`: Script to start all services
- `bin/`: Directory for compiled binaries

## How to Run

1. **Build the project**:
   ```bash
   make
   ```

2. **Start all services** (in one terminal):
   ```bash
   ./run_all.sh
   ```

3. **Start the main server** (in another terminal):
   ```bash
   ./bin/main_server
   ```

4. **Run the client** (in another terminal):
   ```bash
   ./bin/client 127.0.0.1
   ```

5. **Available commands**:
   - `DATE`: Get current date and time
   - `LS <directory>`: List directory contents
   - `CAT <file>`: Display file contents
   - `UPTIME`: Show server uptime
   - `QUIT`: Exit client

6. **Clean up**:
   ```bash
   make clean
   ./kill_all.sh  # To stop running services
   ```