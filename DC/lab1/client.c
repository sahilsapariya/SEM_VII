#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#define PORT 8080
#define MAXLINE 1024

int main() {
    int sock = 0, valread;
    struct sockaddr_in serv_addr;
    char buffer[MAXLINE] = {0};

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Convert IPv4 and IPv6 addresses from text to binary form
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }

    if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("\nConnection Failed \n");
        return -1;
    }

    while (1) {
        char hello[MAXLINE] = {0};
        printf("Enter message: ");
        fgets(hello, MAXLINE, stdin);

        // Remove newline character from fgets
        hello[strcspn(hello, "\n")] = 0;

        send(sock, hello, strlen(hello), 0);
        printf("Message sent\n");

        valread = read(sock, buffer, MAXLINE);
        if (valread <= 0) {
            printf("Server disconnected\n");
            break;
        }

        printf("Server: %s\n", buffer);

        memset(buffer, 0, MAXLINE); // Clear the buffer
    }

    close(sock);

    return 0;
}