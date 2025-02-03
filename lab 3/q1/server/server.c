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

int main() {
    int server_fd, new_sock, valread;
    struct sockaddr_in address;
    char buffer[BUFFER_SIZE] = {0};
    pid_t pid;

    // Creating socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Binding socket
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    // Listening for connections
    if (listen(server_fd, 3) < 0) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }
    printf("Server listening on port %d\n", PORT);

    while (1) {
        // Accepting a client connection
        if ((new_sock = accept(server_fd, NULL, NULL)) < 0) {
            perror("Accept failed");
            exit(EXIT_FAILURE);
        }

        pid = fork();

        if (pid == 0) { // Child process
            close(server_fd);
            printf("Child Process: PID = %d, PPID = %d\n", getpid(), getppid());

            while (1) {
                memset(buffer, 0, BUFFER_SIZE);
                valread = read(new_sock, buffer, BUFFER_SIZE);
                if (valread == 0) {
                    printf("Client disconnected\n");
                    break;
                }
                printf("Client: %s\n", buffer);

                // Sending response to the client
                printf("Server: ");
                fgets(buffer, BUFFER_SIZE, stdin);
                send(new_sock, buffer, strlen(buffer), 0);
            }
            close(new_sock);
            exit(0);
        } else if (pid > 0) { // Parent process
            close(new_sock);  // Parent does not need this
        } else {
            perror("Fork failed");
            exit(EXIT_FAILURE);
        }
    }
    close(server_fd);
    return 0;
}
