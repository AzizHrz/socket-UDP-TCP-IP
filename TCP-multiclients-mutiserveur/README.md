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

## Socket Programming Details

### Main Server (`main_server.c`)
1. **Socket Creation**: `socket(AF_INET, SOCK_STREAM, 0)` creates listening socket
2. **Address Binding**: `bind()` associates with port 9000
3. **Listen Mode**: `listen()` accepts multiple client connections
4. **Thread Creation**: `pthread_create()` spawns thread for each client
5. **Command Routing**: Parses client commands and connects to appropriate service servers
6. **Service Communication**: Uses separate TCP connections to individual service processes
7. **Response Handling**: Forwards service responses back to clients with "--END--" markers

### Service Servers (Individual Processes)
1. **Dedicated Sockets**: Each service binds to its own port (9001-9004)
2. **Service-Specific Logic**:
   - **DateTime Server**: Uses `time()` and `ctime()` for current date/time
   - **ListsDir Server**: Uses `opendir()`/`readdir()` for directory contents
   - **CatDir Server**: Uses `fopen()`/`fread()` for file content display
   - **Elapsed Server**: Tracks server start time for uptime calculation
3. **Request Processing**: Services run in infinite loops, processing requests from main server

### Client (`client.c`)
1. **Socket Creation**: `socket(AF_INET, SOCK_STREAM, 0)` creates TCP socket
2. **Server Connection**: `connect()` establishes connection to main server (port 9000)
3. **Menu Reception**: Reads initial service menu from server
4. **Interactive Commands**: Sends commands like "DATE", "LS path", "CAT file", "UPTIME"
5. **Response Processing**: Accumulates response data until "--END--" marker
6. **Persistent Connection**: Keeps connection open for multiple command exchanges

### Communication Architecture
1. **Service Startup**: `run_all.sh` starts all individual service processes
2. **Main Server**: Listens on port 9000, creates threads for client connections
3. **Command Processing**: Client sends command to main server
4. **Service Routing**: Main server connects to appropriate service based on command
5. **Data Flow**: Service → Main Server → Client (with framing markers)
6. **Concurrency**: Multiple clients served simultaneously via threads
7. **Process Separation**: Services run as separate processes for modularity

### Key Socket Functions
- `pthread_create()`: Thread-based concurrency for multi-client handling
- `connect_to_service()`: Establishes connections to individual service servers
- `read_all()`: Robust reading of service responses
- Message framing with "--END--" for response boundary detection

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