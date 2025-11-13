#!/bin/bash

# Start the server in the background
./sockets_multithread/server &

# Wait for the server to start
sleep 1

# Start the client
./sockets_multithread/client

# Kill the server process
kill %1
