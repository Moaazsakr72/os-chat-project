# Shared Memory + Semaphores Chat
Compile:
g++ writer.cpp -o writer -lrt -pthread
g++ reader.cpp -o reader -lrt -pthread

Run reader:
./reader

Run writer (enter nickname when prompted):
./writer
