#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <fcntl.h>

#define SERVER_IP "127.0.0.1"
#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    // Create socket
    int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Server address setup
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);
    server_address.sin_addr.s_addr = inet_addr(SERVER_IP);

    // Connect to server
    if (connect(client_socket, (struct sockaddr*)&server_address, sizeof(server_address)) < 0) {
        perror("Connection failed");
        exit(EXIT_FAILURE);
    }

    // Open file to send
    FILE *file_to_send = fopen("file_to_send.txt", "r");
    if (file_to_send == NULL) {
        perror("File opening failed");
        exit(EXIT_FAILURE);
    }

    // Send file data to server
    char buffer[BUFFER_SIZE];
    ssize_t bytes_read, bytes_sent;
    while ((bytes_read = fread(buffer, 1, BUFFER_SIZE, file_to_send)) > 0) {
        bytes_sent = write(client_socket, buffer, bytes_read);
        if (bytes_sent == -1) {
            perror("Data sending failed");
            fclose(file_to_send);
            close(client_socket);
            exit(EXIT_FAILURE);
        } else if (bytes_sent < bytes_read) {
            printf("Data sent partially to server\n");
        } else {
            printf("Data sent successfully to server\n");
        }
    }
    
    // Close file and socket
    fclose(file_to_send);
    close(client_socket);

    return 0;
}
