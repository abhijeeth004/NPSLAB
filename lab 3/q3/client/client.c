#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 8080
#define SERVER_IP "127.0.0.1"
#define BUFFER_SIZE 1024

int main() {
    int sock = 0, valread;
    struct sockaddr_in serv_addr;
    char buffer[BUFFER_SIZE];

    // Create socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Convert IP address to binary form
    if (inet_pton(AF_INET, SERVER_IP, &serv_addr.sin_addr) <= 0) {
        perror("Invalid address");
        exit(EXIT_FAILURE);
    }

    // Connect to the server
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Connection failed");
        exit(EXIT_FAILURE);
    }

    // Input string from the user
    printf("Enter a string: ");
    fgets(buffer, BUFFER_SIZE, stdin);
    buffer[strcspn(buffer, "\n")] = '\0';  // Remove newline character

    // Send string to the server
    send(sock, buffer, strlen(buffer), 0);
    printf("String sent to server: %s\n", buffer);

    // Receive sorted numbers from child process
    valread = read(sock, buffer, BUFFER_SIZE);
    buffer[valread] = '\0';
    printf("Sorted numbers from child process: %s\n", buffer);

    // Receive sorted characters from parent process
    valread = read(sock, buffer, BUFFER_SIZE);
    buffer[valread] = '\0';
    printf("Sorted characters from parent process: %s\n", buffer);

    close(sock);
    return 0;
}
