#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 2001

int main() {
    int client_sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (client_sock == -1) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in server_addr = {0};
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    if (inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr) <= 0) {
        perror("invalid address");
        exit(EXIT_FAILURE);
    }

    int read_size;
    char client_message[100] = {0};
    char server_message[100] = {0};

    while (1) {
        printf("Client: ");
        fgets(client_message, sizeof(client_message), stdin);
        sendto(client_sock, client_message, strlen(client_message), 0, (struct sockaddr *)&server_addr, sizeof(server_addr));

        read_size = recvfrom(client_sock, server_message, sizeof(server_message), 0, NULL, NULL);
        printf("Server: %s\n", server_message);

        memset(client_message, 0, sizeof(client_message));
        memset(server_message, 0, sizeof(server_message));
    }

    close(client_sock);
    return 0;
}