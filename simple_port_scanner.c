#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

void scan_port(const char host[], int port) {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        perror("socket");
        return;
    }

    struct sockaddr_in server = {0};
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr(host);
    server.sin_port = htons(port);

    if (connect(sock, (struct sockaddr*)&server, sizeof(server)) < 0) {
        // Port is closed
        printf("Port %d is closed\n", port);
    } else {
        printf("Port %d is open\n", port);
    }

    close(sock);
}

int main() {
    char host[100];
    int choice, port;

    while (1) {
        printf("1. Scan all ports\n");
        printf("2. Scan specific port\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Enter the IP address to scan: ");
                scanf("%s", host);

                for (port = 1; port <= 65535; port++) {
                    scan_port(host, port);
                }

                break;
            case 2:
                printf("Enter the IP address to scan: ");
                scanf("%s", host);

                printf("Enter the port to scan: ");
                scanf("%d", &port);

                scan_port(host, port);
                break;
            case 5:
                printf("Exiting...\n");
                exit(0);
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }

    return 0;
}

