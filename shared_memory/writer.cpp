// writer.cpp
// Compile: g++ writer.cpp -o writer -lrt -pthread
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
    Message buf[16]; // capacity fixed 16
};

int main() {
    cout << "Nickname: ";
    string nick; getline(cin, nick);
    int fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
    ftruncate(fd, sizeof(ShmRing));
    void* ptr = mmap(NULL, sizeof(ShmRing), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    ShmRing *ring = (ShmRing*)ptr;

    // initialize only first writer (simple approach)
    ring->capacity = 16;

    sem_t *mutex = sem_open(SEM_MUTEX, O_CREAT, 0666, 1);
    sem_t *empty = sem_open(SEM_EMPTY, O_CREAT, 0666, ring->capacity);
    sem_t *full  = sem_open(SEM_FULL,  O_CREAT, 0666, 0);

    cout << "Type messages, /quit to exit\n";
    string line;
    while (true) {
        getline(cin, line);
        if (line == "/quit") break;

        sem_wait(empty);
        sem_wait(mutex);

        // write at head
        Message &m = ring->buf[ring->head % ring->capacity];
        strncpy(m.nick, nick.c_str(), sizeof(m.nick)-1);
        m.nick[sizeof(m.nick)-1] = 0;
        strncpy(m.text, line.c_str(), sizeof(m.text)-1);
        m.text[sizeof(m.text)-1] = 0;
        ring->head++;

        sem_post(mutex);
        sem_post(full);
    }

    // cleanup (do not unlink if multiple users may run)
    munmap(ptr, sizeof(ShmRing));
    close(fd);
    sem_close(mutex); sem_close(empty); sem_close(full);
    return 0;
}
