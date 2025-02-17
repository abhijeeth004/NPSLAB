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

int main() {
    int client_socket, data, parity_bit;
    struct sockaddr_in server_addr;

    // Create socket
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Set server address
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    // Connect to the server
    if (connect(client_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection failed");
        close(client_socket);
        exit(EXIT_FAILURE);
    }

    // Get 8-bit data from the user
    printf("Enter an 8-bit binary data (0 to 255): ");
    scanf("%d", &data);

    // Calculate parity bit
    parity_bit = calculate_parity(data, 8);

    // Send data and parity bit to the server
    send(client_socket, &data, sizeof(data), 0);
    send(client_socket, &parity_bit, sizeof(parity_bit), 0);

    printf("Sender: Data sent: %d (with parity bit %d)\n", data, parity_bit);

    close(client_socket);
    return 0;
}
