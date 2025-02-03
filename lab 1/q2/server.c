#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define MAX_LEN 1024

// Function to check if a string is a palindrome
int is_palindrome(char *str) {
    int len = strlen(str);
    for (int i = 0; i < len / 2; i++) {
        if (str[i] != str[len - i - 1]) {
            return 0; // Not a palindrome
        }
    }
    return 1; // Palindrome
}

// Function to count the occurrences of vowels
void count_vowels(char *str, int *vowels_count) {
    for (int i = 0; str[i] != '\0'; i++) {
        char c = tolower(str[i]);
        if (c == 'a') vowels_count[0]++;
        else if (c == 'e') vowels_count[1]++;
        else if (c == 'i') vowels_count[2]++;
        else if (c == 'o') vowels_count[3]++;
        else if (c == 'u') vowels_count[4]++;
    }
}

int main() {
    int sockfd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len = sizeof(client_addr);
    char buffer[MAX_LEN];
    
    // Create a UDP socket
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);
    
    // Bind the socket
    if (bind(sockfd, (const struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    printf("Server is listening on port %d...\n", PORT);
    
    while (1) {
        // Receive message from client
        int n = recvfrom(sockfd, (char *)buffer, MAX_LEN, MSG_WAITALL, (struct sockaddr *) &client_addr, &addr_len);
        buffer[n] = '\0';
        
        if (strcmp(buffer, "Halt") == 0) {
            printf("Server terminating...\n");
            break;
        }

        // Check if the string is a palindrome
        int palindrome = is_palindrome(buffer);

        // Calculate length and vowel counts
        int vowels_count[5] = {0}; // A, E, I, O, U
        count_vowels(buffer, vowels_count);
        int length = strlen(buffer);

        // Prepare the response
        char response[MAX_LEN];
        snprintf(response, MAX_LEN, "Palindrome: %s\nLength: %d\nVowel Counts - A: %d, E: %d, I: %d, O: %d, U: %d", 
                 palindrome ? "Yes" : "No", length, vowels_count[0], vowels_count[1], vowels_count[2], vowels_count[3], vowels_count[4]);
        
        // Send the result to the client
        sendto(sockfd, (const char *)response, strlen(response), MSG_CONFIRM, (const struct sockaddr *) &client_addr, addr_len);
    }
    
    close(sockfd);
    return 0;
}
