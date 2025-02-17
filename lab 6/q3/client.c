#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

// Function to encode data using Hamming (7, 4) code
void encode_hamming(unsigned char *data, unsigned char *encoded_data) {
    // Assuming data is 4 bits
    // Set the data bits (3, 5, 6, 7)
    encoded_data[2] = (data[0] >> 3) & 1;  // D1
    encoded_data[4] = (data[0] >> 2) & 1;  // D2
    encoded_data[5] = (data[0] >> 1) & 1;  // D3
    encoded_data[6] = data[0] & 1;         // D4

    // Set the parity bits (P1, P2, P4)
    encoded_data[0] = (encoded_data[2] ^ encoded_data[4] ^ encoded_data[6]) % 2;  // P1
    encoded_data[1] = (encoded_data[2] ^ encoded_data[5] ^ encoded_data[6]) % 2;  // P2
    encoded_data[3] = (encoded_data[4] ^ encoded_data[5] ^ encoded_data[6]) % 2;  // P4
}

int main() {
    int client_socket;
    struct sockaddr_in server_addr;
    unsigned char data[1];  // 4 bits of data
    unsigned char encoded_data[7];  // 7 bits (4 data + 3 parity)

    // Get data input from the user (1 byte, 4 bits of data)
    printf("Enter 4-bit data (0-15): ");
    scanf("%hhu", &data[0]);

    // Encode the data using Hamming code
    encode_hamming(data, encoded_data);

    // Simulate an error by flipping a random bit (for testing purposes)
    encoded_data[2] ^= 1;  // Flip the bit at position 3 (just an example)

    // Create socket
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Set up server address
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

    // Send the encoded data (7 bits) to the server
    send(client_socket, encoded_data, sizeof(encoded_data), 0);

    printf("Data sent: %d%d%d%d%d%d%d\n", encoded_data[0], encoded_data[1], encoded_data[2], encoded_data[3], encoded_data[4], encoded_data[5], encoded_data[6]);

    close(client_socket);
    return 0;
}
