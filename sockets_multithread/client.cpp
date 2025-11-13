// client.cpp
// Compile: g++ -pthread client.cpp -o client
#include <bits/stdc++.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
using namespace std;

const int PORT = 54000;

int sockfd;

void recv_thread() {
    char buf[512];
    while (true) {
        ssize_t n = recv(sockfd, buf, sizeof(buf)-1, 0);
        if (n <= 0) { cout << "Disconnected from server\n"; exit(0); }
        buf[n] = 0;
        cout << buf; // server sends full lines with newline
        cout.flush();
    }
}

int main() {
    string server_ip;
    cout << "Server IP: ";
    getline(cin, server_ip);
    cout << "Nickname: ";
    string nick; getline(cin, nick);

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) { perror("socket"); return 1; }

    sockaddr_in serv{};
    serv.sin_family = AF_INET;
    serv.sin_port = htons(PORT);
    if (inet_pton(AF_INET, server_ip.c_str(), &serv.sin_addr) <= 0) { cerr << "Invalid IP\n"; return 1; }

    if (connect(sockfd, (sockaddr*)&serv, sizeof(serv)) < 0) { perror("connect"); return 1; }
    cout << "Connected to server. Type messages and press Enter.\n";

    thread t(recv_thread);
    t.detach();

    string line;
    while (getline(cin, line)) {
        if (line == "/quit") break;
        string out = "[" + nick + "] " + line + "\n";
        send(sockfd, out.c_str(), out.size(), 0);
    }
    close(sockfd);
    return 0;
}
