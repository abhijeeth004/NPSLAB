#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "crc_calculate.h"

void handle_client(int client_socket) {
    unsigned char buffer[1024];
    unsigned short received_crc, computed_crc;
    int length;

    // Receive the length of the data
    recv(client_socket, &length, sizeof(length), 0);

    // Receive data and CRC from the client
    recv(client_socket, buffer, length, 0);
    recv(client_socket, &received_crc, sizeof(received_crc), 0);

    // Calculate CRC-16 (You can modify this to CRC-12 or CRC-CCITT as needed)
    computed_crc = crc16(buffer, length);

    if (computed_crc == received_crc) {
        printf("Receiver: Data received correctly!\n");
    } else {
        printf("Receiver: Data corruption detected! (CRC mismatch)\n");
    }

    close(client_socket);
}

int main() {
    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);

    // Create socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Set server address
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(8080);

    // Bind the socket
    if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Binding failed");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(server_socket, 5) < 0) {
        perror("Listening failed");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    printf("Server is waiting for connections...\n");

    // Accept client connections
    client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &client_len);
    if (client_socket < 0) {
        perror("Accept failed");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    // Handle client request
    handle_client(client_socket);

    close(server_socket);
    return 0;
}
