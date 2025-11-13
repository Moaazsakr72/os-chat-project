// reader.cpp
// Compile: g++ reader.cpp -o reader -lrt -pthread
#include <bits/stdc++.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <unistd.h>
using namespace std;

const char *SHM_NAME = "/os_chat_shm";
const char *SEM_MUTEX = "/os_chat_mutex";
const char *SEM_EMPTY = "/os_chat_empty";
const char *SEM_FULL  = "/os_chat_full";

struct Message {
    char nick[32];
    char text[256];
};

struct ShmRing {
    size_t capacity;
    size_t head;
    size_t tail;
    Message buf[16];
};

int main() {
    int fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
    ftruncate(fd, sizeof(ShmRing));
    void* ptr = mmap(NULL, sizeof(ShmRing), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    ShmRing *ring = (ShmRing*)ptr;

    sem_t *mutex = sem_open(SEM_MUTEX, O_CREAT, 0666, 1);
    sem_t *empty = sem_open(SEM_EMPTY, O_CREAT, 0666, ring->capacity);
    sem_t *full  = sem_open(SEM_FULL,  O_CREAT, 0666, 0);

    cout << "Reader started. Waiting for messages...\n";

    while (true) {
        sem_wait(full);
        sem_wait(mutex);

        Message m = ring->buf[ring->tail % ring->capacity];
        ring->tail++;

        sem_post(mutex);
        sem_post(empty);

        cout << "[" << m.nick << "] " << m.text << "\n";
        cout.flush();
    }

    munmap(ptr, sizeof(ShmRing));
    close(fd);
    sem_close(mutex); sem_close(empty); sem_close(full);
    return 0;
}
