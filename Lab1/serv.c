#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>

#define SERVER_IP "127.0.0.1"
#define PORT 8080
#define BUFFER_SIZE 1024 * 10

int main() {
    // Create server socket
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    } else {
        printf("[+] Server socket created successfully\n");
    }

    // Set up server address
    struct sockaddr_in server_addr, client_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);

    // Bind server socket to address
    if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }
    printf("[+] Server socket bound successfully\n");

    // Listen for connections
    if (listen(server_fd, 10) < 0) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }
    printf("[+] Listening for connections...\n");

    // Accept connection from client
    socklen_t client_addr_len = sizeof(client_addr);
    int new_socket = accept(server_fd, (struct sockaddr*)&client_addr, &client_addr_len);
    if (new_socket < 0) {
        perror("Accept failed");
        exit(EXIT_FAILURE);
    }
    printf("[+] Connection accepted from client\n");

    // Open or create file for writing
    int file_fd = open("test2.txt", O_WRONLY | O_CREAT | O_TRUNC, 0666);
    if (file_fd == -1) {
        perror("File opening or creation failed");
        exit(EXIT_FAILURE);
    }
    printf("[+] File opened or created successfully\n");

    // Receive data from client and write to file
    char buffer[BUFFER_SIZE];
    ssize_t bytes_received;
    while ((bytes_received = recv(new_socket, buffer, BUFFER_SIZE, 0)) > 0) {
        ssize_t bytes_written = write(file_fd, buffer, bytes_received);
        if (bytes_written != bytes_received) {
            perror("Error writing to file");
            close(file_fd);
            close(new_socket);
            exit(EXIT_FAILURE);
        }
    }
    if (bytes_received == -1) {
        perror("Error receiving data from client");
        close(file_fd);
        close(new_socket);
        exit(EXIT_FAILURE);
    }

    printf("[+] Data received and written to 'test2.txt' successfully\n");

    // Close file and socket
    close(file_fd);
    close(new_socket);

    return 0;
}
