#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "crc_calculate.h"

int main() {
    int client_socket;
    struct sockaddr_in server_addr;
    unsigned char buffer[1024];
    unsigned short crc;
    int length;

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

    // Get data from user
    printf("Enter the data to send: ");
    fgets((char*)buffer, sizeof(buffer), stdin);
    length = strlen((char*)buffer);
    if (buffer[length - 1] == '\n') buffer[length - 1] = '\0';  // Remove newline character

    // Calculate CRC-16 for the data
    crc = crc16(buffer, length);

    // Send the length of the data, the data, and the CRC to the server
    send(client_socket, &length, sizeof(length), 0);
    send(client_socket, buffer, length, 0);
    send(client_socket, &crc, sizeof(crc), 0);

    printf("Sender: Data sent with CRC-16 checksum: %x\n", crc);

    close(client_socket);
    return 0;
}
