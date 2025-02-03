#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 8080
#define MAX_BUF 1024
#define DB_FILE "database.txt"

typedef struct {
    char domain[256];
    char ip[16];
} DNSRecord;

// Function to load DNS records from a file into memory
int load_dns_records(DNSRecord *records) {
    FILE *file = fopen(DB_FILE, "r");
    if (file == NULL) {
        perror("Failed to open database.txt");
        return -1;
    }

    int i = 0;
    while (fscanf(file, "%s %s", records[i].domain, records[i].ip) != EOF) {
        i++;
    }

    fclose(file);
    return i;  // Return number of records loaded
}

// Function to handle DNS query
void handle_dns_query(int client_socket, struct sockaddr_in *client_addr, socklen_t *client_len, DNSRecord *records, int num_records) {
    char buffer[MAX_BUF];
    int len = recvfrom(client_socket, buffer, MAX_BUF, 0, (struct sockaddr *)client_addr, client_len);  // Fix here
    if (len == -1) {
        perror("recvfrom failed");
        return;
    }

    buffer[len] = '\0';  // Null terminate the received string
    printf("Received DNS query for domain: %s\n", buffer);

    // Search for the domain in the records
    for (int i = 0; i < num_records; i++) {
        if (strcmp(buffer, records[i].domain) == 0) {
            // Found the domain, send the corresponding IP address to the client
            sendto(client_socket, records[i].ip, strlen(records[i].ip), 0, (struct sockaddr *)client_addr, *client_len);
            printf("Sent IP address: %s\n", records[i].ip);
            return;
        }
    }

    // Domain not found, send error message
    char *error_message = "Domain not found!";
    sendto(client_socket, error_message, strlen(error_message), 0, (struct sockaddr *)client_addr, *client_len);
    printf("Domain not found: %s\n", buffer);
}

int main() {
    int server_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);  // Correctly initialize client_len
    
    // Load DNS records from the file
    DNSRecord records[100];
    int num_records = load_dns_records(records);
    if (num_records == -1) {
        return 1;
    }

    // Create a UDP socket
    if ((server_socket = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Socket creation failed");
        return 1;
    }

    // Setup the server address
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // Bind the socket
    if (bind(server_socket, (const struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        return 1;
    }

    printf("DNS Server is listening on port %d...\n", PORT);

    // Listen for incoming requests
    while (1) {
        handle_dns_query(server_socket, &client_addr, &client_len, records, num_records);  // Correct the function call
    }

    close(server_socket);
    return 0;
}
