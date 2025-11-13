# C/C++ Operating Systems Project

This project contains two C/C++ systems that demonstrate inter-process communication:

- **Sockets Multithread:** A client-server application that uses sockets for communication.
- **Shared Memory:** A writer-reader application that uses shared memory for communication.

## How to Compile

These instructions are for Linux and Windows WSL.

### Prerequisites

- g++ compiler
- make

### Build

To build both systems, run the following command from the root of the project:

```bash
make
```

This will create the following executables:

- `sockets_multithread/server`
- `sockets_multithread/client`
- `shared_memory/writer`
- `shared_memory/reader`

## How to Run

### Sockets Multithread

1. **Start the server:**
   ```bash
   ./sockets_multithread/server
   ```

2. **Open a new terminal and start the client:**
   ```bash
   ./sockets_multithread/client
   ```

### Shared Memory

1. **Start the reader:**
   ```bash
   ./shared_memory/reader
   ```

2. **Open a new terminal and start the writer:**
   ```bash
   ./shared_memory/writer
   ```
