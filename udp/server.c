#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define SERVER_PORT 2001

int main() {
    int server_sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (server_sock == -1) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in server_addr = {0};
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(SERVER_PORT);

    if (bind(server_sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    printf("Server ready, waiting for client...\n");

    int client_addr_len, read_size;
    struct sockaddr_in client_addr = {0};
    char client_message[100] = {0};
    char server_message[100] = {0};

    while (1) {
        client_addr_len = sizeof(client_addr);
        read_size = recvfrom(server_sock, client_message, sizeof(client_message), 0, (struct sockaddr *)&client_addr, &client_addr_len);
        printf("Client: %s\n", client_message);

        printf("Server: ");
        fgets(server_message, sizeof(server_message), stdin);
        sendto(server_sock, server_message, strlen(server_message), 0, (struct sockaddr *)&client_addr, client_addr_len);

        memset(client_message, 0, sizeof(client_message));
        memset(server_message, 0, sizeof(server_message));
    }

    close(server_sock);
    return 0;
}