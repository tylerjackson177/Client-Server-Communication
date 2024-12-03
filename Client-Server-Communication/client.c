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

int server_fd;

int main(void) {
    struct sockaddr_in addr;
    pthread_t read_thread, write_thread;

    // Create socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        perror("socket");
        exit(1);
    }

    // Configure server address
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(PORT);

    // Connect to server
    if (connect(server_fd, (struct sockaddr *)&addr, sizeof(addr)) == -1) {
        perror("connect");
        close(server_fd);
        exit(2);
    }

    printf("Connected to server.\n");

    // Create threads for reading and writing
    pthread_create(&read_thread, NULL, handle_read, NULL);
    pthread_create(&write_thread, NULL, handle_write, NULL);

    // Wait for threads to finish
    pthread_join(read_thread, NULL);
    pthread_join(write_thread, NULL);

    close(server_fd);
    return 0;
}

void *handle_read(void *arg) {
    char buffer[BUF_SIZE];
    while (1) {
        ssize_t rcount = read(server_fd, buffer, BUF_SIZE);
        if (rcount > 0) {
            buffer[rcount] = '\0';
            printf("Server: %s\n", buffer);
        } else {
            break;
        }
    }
    return NULL;
}

void *handle_write(void *arg) {
    char buffer[BUF_SIZE];
    while (1) {
        printf("Client: ");
        if (fgets(buffer, BUF_SIZE, stdin) != NULL) {
            write(server_fd, buffer, strlen(buffer));
        }
    }
    return NULL;
}
