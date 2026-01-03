#include <iostream>
#include <fstream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 4096

int main() {
    int sock;
    struct sockaddr_in server_addr;

    sock = socket(AF_INET, SOCK_STREAM, 0);

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);

    // Change this to sender's IP
    inet_pton(AF_INET, "192.168.64.2", &server_addr.sin_addr);

    connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr));

    long file_size;
    recv(sock, &file_size, sizeof(file_size), 0);

    std::ofstream file("received.txt", std::ios::binary);

    char buffer[BUFFER_SIZE];
    long received = 0;

    while (received < file_size) {
        int bytes = recv(sock, buffer, BUFFER_SIZE, 0);
        file.write(buffer, bytes);
        received += bytes;
    }

    std::cout << "File received successfully.\n";

    close(sock);
    return 0;
}
