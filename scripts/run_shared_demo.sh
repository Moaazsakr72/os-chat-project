#!/bin/bash

# Start the reader in the background
./shared_memory/reader &

# Wait for the reader to start
sleep 1

# Start the writer
./shared_memory/writer

# Kill the reader process
kill %1
