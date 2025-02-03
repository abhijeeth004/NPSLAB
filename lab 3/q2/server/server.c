#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

// Function to swap characters
void swap(char *x, char *y) {
    char temp = *x;
    *x = *y;
    *y = temp;
}

// Function to print all permutations of the string
void permute(char *str, int l, int r) {
    if (l == r) {
        printf("%s\n", str);
    } else {
        for (int i = l; i <= r; i++) {
            swap((str + l), (str + i));
            permute(str, l + 1, r);
            swap((str + l), (str + i)); // backtrack
        }
    }
}

int main() {
    int sockfd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);
    char buffer[BUFFER_SIZE];

    // Create UDP socket
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // Bind the socket
    if (bind(sockfd, (const struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    printf("Server is listening on port %d\n", PORT);

    while (1) {
        // Receive the string from the client
        int n = recvfrom(sockfd, (char *)buffer, BUFFER_SIZE, MSG_WAITALL,
                         (struct sockaddr *)&client_addr, &client_len);
        buffer[n] = '\0';
        printf("Received string from client: %s\n", buffer);

        // Print all permutations of the string
        printf("Permutations of the string '%s' are:\n", buffer);
        permute(buffer, 0, n - 1);

        // Send acknowledgment to the client
        char *ack = "Permutations printed. Server ready for next request.";
        sendto(sockfd, ack, strlen(ack), MSG_CONFIRM, (const struct sockaddr *)&client_addr, client_len);
    }

    close(sockfd);
    return 0;
}
