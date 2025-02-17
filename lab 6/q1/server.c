#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int calculate_parity(int data, int bit_count) {
    int ones_count = 0;
    for (int i = 0; i < bit_count; i++) {
        if ((data >> i) & 1) {
            ones_count++;
        }
    }
    return ones_count % 2 == 0 ? 0 : 1;  // Even parity
}

void handle_client(int client_socket) {
    int received_data, parity_bit;
    
    // Receive data from the client
    recv(client_socket, &received_data, sizeof(received_data), 0);
    recv(client_socket, &parity_bit, sizeof(parity_bit), 0);
    
    // Check the parity bit
    int calculated_parity = calculate_parity(received_data, 8);
    if (calculated_parity == parity_bit) {
        printf("Receiver: Data received correctly: %d\n", received_data);
    } else {
        printf("Receiver: Data corruption detected! (Parity mismatch)\n");
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
