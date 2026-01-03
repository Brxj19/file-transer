#include <iostream>
#include <fstream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 4096

int main() {
    int server_fd, client_socket;
    struct sockaddr_in address;
    socklen_t addr_len = sizeof(address);

    // Create socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY; // accept any incoming IP
    address.sin_port = htons(PORT);

    bind(server_fd, (struct sockaddr*)&address, sizeof(address));
    listen(server_fd, 1);

    std::cout << "Waiting for connection...\n";
    client_socket = accept(server_fd, (struct sockaddr*)&address, &addr_len);
    std::cout << "Client connected!\n";

    std::ifstream file("send.txt", std::ios::binary);
    if (!file) {
        std::cerr << "File not found!\n";
        return 1;
    }

    file.seekg(0, std::ios::end);
    long file_size = file.tellg();
    file.seekg(0, std::ios::beg);

    // Send file size
    send(client_socket, &file_size, sizeof(file_size), 0);

    char buffer[BUFFER_SIZE];
    while (!file.eof()) {
        file.read(buffer, BUFFER_SIZE);
        send(client_socket, buffer, file.gcount(), 0);
    }

    std::cout << "File sent successfully.\n";

    close(client_socket);
    close(server_fd);
    return 0;
}
