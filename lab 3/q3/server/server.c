#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <ctype.h>

#define PORT 8080
#define BUFFER_SIZE 1024

// Function to compare for sorting numbers in ascending order
int compare_numbers(const void *a, const void *b) {
    return (*(char *)a - *(char *)b);
}

// Function to compare for sorting characters in descending order
int compare_characters(const void *a, const void *b) {
    return (*(char *)b - *(char *)a);
}

int main() {
    int server_fd, new_sock, valread;
    struct sockaddr_in address;
    char buffer[BUFFER_SIZE];
    pid_t pid;
    char num_str[BUFFER_SIZE], char_str[BUFFER_SIZE];
    int num_len = 0, char_len = 0;

    // Create socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Bind socket
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    // Listen for connections
    if (listen(server_fd, 3) < 0) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d\n", PORT);

    // Accept a client connection
    if ((new_sock = accept(server_fd, NULL, NULL)) < 0) {
        perror("Accept failed");
        exit(EXIT_FAILURE);
    }

    valread = read(new_sock, buffer, BUFFER_SIZE);
    buffer[valread] = '\0';  // Null-terminate the received string

    printf("Received string from client: %s\n", buffer);

    // Separate numbers and characters
    for (int i = 0; i < valread; i++) {
        if (isdigit(buffer[i])) {
            num_str[num_len++] = buffer[i];
        } else if (isalpha(buffer[i])) {
            char_str[char_len++] = buffer[i];
        }
    }
    num_str[num_len] = '\0';
    char_str[char_len] = '\0';

    // Create a child process to sort numbers in ascending order
    pid = fork();
    if (pid == 0) {  // Child process
        qsort(num_str, num_len, sizeof(char), compare_numbers);
        printf("Child Process (PID: %d): Sorted numbers = %s\n", getpid(), num_str);
        send(new_sock, num_str, strlen(num_str), 0);  // Send sorted numbers to client
        exit(0);
    } else if (pid > 0) {  // Parent process
        qsort(char_str, char_len, sizeof(char), compare_characters);
        printf("Parent Process (PID: %d): Sorted characters = %s\n", getpid(), char_str);
        send(new_sock, char_str, strlen(char_str), 0);  // Send sorted characters to client
    } else {
        perror("Fork failed");
        exit(EXIT_FAILURE);
    }

    close(new_sock);
    close(server_fd);
    return 0;
}
