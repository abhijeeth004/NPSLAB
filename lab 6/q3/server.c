#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

// Function to detect and correct the error using Hamming code
void detect_and_correct_error(unsigned char *data) {
    int error_position = 0;
    int parity_bits[3];

    // Check parity bits
    parity_bits[0] = (data[0] ^ data[2] ^ data[4] ^ data[6]) % 2;  // Parity bit P1
    parity_bits[1] = (data[1] ^ data[2] ^ data[5] ^ data[6]) % 2;  // Parity bit P2
    parity_bits[2] = (data[3] ^ data[4] ^ data[5] ^ data[6]) % 2;  // Parity bit P4

    // Determine error position (if any)
    error_position = parity_bits[0] * 1 + parity_bits[1] * 2 + parity_bits[2] * 4;

    if (error_position != 0) {
        printf("Error detected at position: %d\n", error_position);
        data[error_position - 1] ^= 1;  // Flip the erroneous bit to correct it
        printf("Error corrected. Corrected data: %d%d%d%d%d%d%d\n", data[0], data[1], data[2], data[3], data[4], data[5], data[6]);
    } else {
        printf("No error detected. Data is correct: %d%d%d%d%d%d%d\n", data[0], data[1], data[2], data[3], data[4], data[5], data[6]);
    }
}

int main() {
    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);
    unsigned char received_data[7];  // 7-bit data (4 data bits + 3 parity bits)

    // Create socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Set up server address
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

    printf("Server waiting for connections...\n");

    // Accept client connections
    client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &client_len);
    if (client_socket < 0) {
        perror("Client accept failed");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    // Receive 7-bit data from the client
    recv(client_socket, received_data, sizeof(received_data), 0);

    // Detect and correct errors
    detect_and_correct_error(received_data);

    close(client_socket);
    close(server_socket);
    return 0;
}
