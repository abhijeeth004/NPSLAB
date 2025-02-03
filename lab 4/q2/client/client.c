#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define MAX_BUF 1024

int main() {
    int client_socket;
    struct sockaddr_in server_addr;
    char domain[MAX_BUF];
    char response[MAX_BUF];

    // Create a UDP socket
    if ((client_socket = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Socket creation failed");
        return 1;
    }

    // Setup the server address
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);

    // Use inet_pton to convert IP address string to binary format
    if (inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr) <= 0) {
        perror("Invalid address/Address not supported");
        close(client_socket);
        return 1;
    }

    // Input the domain name from the user
    printf("Enter the domain name to query: ");
    scanf("%s", domain);

    // Send the domain name to the server
    ssize_t sent_bytes = sendto(client_socket, domain, strlen(domain), 0, 
                                (struct sockaddr *)&server_addr, sizeof(server_addr));
    if (sent_bytes == -1) {
        perror("Send failed");
        close(client_socket);
        return 1;
    }

    // Receive the response from the server
    ssize_t len = recvfrom(client_socket, response, MAX_BUF, 0, NULL, NULL);
    if (len == -1) {
        perror("Receive failed");
        close(client_socket);
        return 1;
    }

    response[len] = '\0';  // Null-terminate the received string

    // Print the response (IP address or error message)
    printf("Response from server: %s\n", response);

    // Close the client socket
    close(client_socket);
    return 0;
}
