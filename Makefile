.PHONY: all sockets shared clean

all: sockets shared

sockets:
	g++ -pthread sockets_multithread/server.cpp -o sockets_multithread/server
	g++ -pthread sockets_multithread/client.cpp -o sockets_multithread/client

shared:
	g++ shared_memory/writer.cpp -o shared_memory/writer -lrt -pthread
	g++ shared_memory/reader.cpp -o shared_memory/reader -lrt -pthread

clean:
	rm -f sockets_multithread/server sockets_multithread/client shared_memory/writer shared_memory/reader
