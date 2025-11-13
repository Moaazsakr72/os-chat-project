# OS Chat Project - Summary

## Systems
1. Sockets + Multithreading
   - TCP server handles clients with threads.
   - Clients send: "[nick] message" lines.
   - Broadcast model: server forwards each message to all connected clients.

2. Shared Memory + Semaphores
   - POSIX shared memory holds a ring buffer of messages.
   - Semaphores: mutex, empty, full for producer-consumer.
   - Writer processes produce messages; reader consumes and prints.

## How to run
See respective README files in folders. Use g++ commands provided.

## Demo script
See docs/demo_script.txt
