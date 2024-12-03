#include <sys/socket.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <pthread.h>

#define PORT 30041
#define BUF_SIZE 1024

void *handle_read(void *arg);
void *handle_write(void *arg);

int client_fd;

int main(void) {
    int listen_fd, err, opt;
    socklen_t addrlen;
    struct sockaddr_in addr;

    pthread_t read_thread, write_thread;

    // Create socket
    listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_fd == -1) {
        perror("socket");
        exit(1);
    }

    // Set socket options
    opt = 1;
    setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    // Configure server address
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(PORT);

    addrlen = sizeof(addr);

    // Bind socket
    if (bind(listen_fd, (struct sockaddr *)&addr, addrlen) == -1) {
        perror("bind");
        close(listen_fd);
        exit(2);
    }

    // Listen for connections
    if (listen(listen_fd, 5) == -1) {
        perror("listen");
        close(listen_fd);
        exit(3);
    }

    printf("Server is listening on port %d...\n", PORT);

    // Accept client connection
    client_fd = accept(listen_fd, NULL, NULL);
    if (client_fd <= 0) {
        perror("accept");
        close(listen_fd);
        exit(4);
    }

    printf("Client connected.\n");

    // Create threads for reading and writing
    pthread_create(&read_thread, NULL, handle_read, NULL);
    pthread_create(&write_thread, NULL, handle_write, NULL);

    // Wait for threads to finish
    pthread_join(read_thread, NULL);
    pthread_join(write_thread, NULL);

    close(client_fd);
    close(listen_fd);
    return 0;
}

void *handle_read(void *arg) {
    char buffer[BUF_SIZE];
    while (1) {
        ssize_t rcount = read(client_fd, buffer, BUF_SIZE);
        if (rcount > 0) {
            buffer[rcount] = '\0';
            printf("Client: %s\n", buffer);
        } else {
            break;
        }
    }
    return NULL;
}

void *handle_write(void *arg) {
    char buffer[BUF_SIZE];
    while (1) {
        printf("Server: ");
        if (fgets(buffer, BUF_SIZE, stdin) != NULL) {
            write(client_fd, buffer, strlen(buffer));
        }
    }
    return NULL;
}
