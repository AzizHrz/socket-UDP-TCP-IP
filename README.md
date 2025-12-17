# UNIX-MidW Socket Programming Projects

This repository contains various socket programming examples in C for UNIX/Linux systems, demonstrating different communication patterns using TCP and UDP protocols.

## Projects Overview

1. **client-server-TCP**: Basic TCP client-server communication with single client and server exchanging messages.

2. **client-server-UDP**: Basic UDP client-server communication with single client and server exchanging messages.

3. **TCP-Monoclient-Monoservice**: TCP client-server setup with one client connecting to a server that provides a single service (listing directory contents).

4. **TCP-fork-based-Multiclient-Monoserveur**: TCP server that handles multiple clients concurrently using fork() for process-based concurrency, providing directory listing service.

5. **TCP-multiclients-mutiserveur**: Advanced multiservice TCP server handling multiple clients with multiple specialized services (date/time, directory listing, file content, uptime) using threads.


Each project folder contains source code, Makefiles for compilation, and detailed README with setup and execution instructions.
