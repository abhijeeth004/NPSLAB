#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define MAX_LEN 1024

int main() {
    int sockfd;
    struct sockaddr_in server_addr;
    char buffer[MAX_LEN];
    
    // Create a UDP socket
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    printf("Client started. Type 'Halt' to exit.\n");

    while (1) {
        printf("Enter string: ");
        fgets(buffer, MAX_LEN, stdin);
        buffer[strcspn(buffer, "\n")] = '\0'; // Remove newline character
        
        // Send the string to the server
        sendto(sockfd, (const char *)buffer, strlen(buffer), MSG_CONFIRM, (const struct sockaddr *) &server_addr, sizeof(server_addr));

        // If the user types "Halt", exit the loop
        if (strcmp(buffer, "Halt") == 0) {
            break;
        }

        // Receive the result from the server
        int n = recvfrom(sockfd, (char *)buffer, MAX_LEN, MSG_WAITALL, NULL, NULL);
        buffer[n] = '\0';

        // Display the received result
        printf("Server response: \n%s\n", buffer);
    }

    close(sockfd);
    return 0;
}
