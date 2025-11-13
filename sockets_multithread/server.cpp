// server.cpp
// Compile: g++ -pthread server.cpp -o server
#include <bits/stdc++.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
using namespace std;

const int PORT = 54000;
const int MAX_CLIENTS = 100;
int server_fd;
vector<int> clients;
mutex clients_mtx;

void broadcast_message(const string &msg, int exclude_fd = -1) {
    lock_guard<mutex> lk(clients_mtx);
    for (int c : clients) {
        if (c == exclude_fd) continue;
        send(c, msg.c_str(), msg.size(), 0);
    }
}

void handle_client(int client_fd) {
    char buf[512];
    while (true) {
        ssize_t bytes = recv(client_fd, buf, sizeof(buf)-1, 0);
        if (bytes <= 0) break;
        buf[bytes] = 0;
        string msg = string(buf);
        // Simple server-side log
        cout << "Recv from " << client_fd << ": " << msg;
        broadcast_message(msg, client_fd);
    }
    // client disconnected
    close(client_fd);
    {
        lock_guard<mutex> lk(clients_mtx);
        clients.erase(remove(clients.begin(), clients.end(), client_fd), clients.end());
    }
    cout << "Client " << client_fd << " disconnected\n";
}

int main() {
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) { perror("socket"); return 1; }

    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(PORT);

    if (bind(server_fd, (sockaddr*)&addr, sizeof(addr)) < 0) { perror("bind"); return 1; }
    if (listen(server_fd, 10) < 0) { perror("listen"); return 1; }

    cout << "Server listening on port " << PORT << "\n";

    while (true) {
        sockaddr_in client_addr;
        socklen_t len = sizeof(client_addr);
        int client_fd = accept(server_fd, (sockaddr*)&client_addr, &len);
        if (client_fd < 0) { perror("accept"); continue; }
        {
            lock_guard<mutex> lk(clients_mtx);
            clients.push_back(client_fd);
        }
        cout << "New client " << client_fd << " from " << inet_ntoa(client_addr.sin_addr) << "\n";
        thread t(handle_client, client_fd);
        t.detach();
    }

    close(server_fd);
    return 0;
}
