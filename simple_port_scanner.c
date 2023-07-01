#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

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
    int choice, first_port, endPort;
    char port_List[100];

    while (1) {
        printf("1. Scan all ports\n");
        printf("2. Scan specific port\n");
        printf("3. Scan a range of ports\n");
        printf("4. Scan a list of ports\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Enter the IP address to scan: ");
                scanf("%s", host);

                for (int port = 1; port <= 65535; port++) {
                    scan_port(host, port);
                }

                break;
            case 2:
                printf("Enter the IP address to scan: ");
                scanf("%s", host);

                printf("Enter the port to scan: ");
                scanf("%d", &first_port);

                scan_port(host, first_port);
                break;
            case 3:
                printf("Enter the IP address to scan: ");
                scanf("%s", host);

                printf("Enter the starting port of the range: ");
                scanf("%d", &first_port);

                printf("Enter the ending port of the range: ");
                scanf("%d", &endPort);

                for (int port = first_port; port <= endPort; port++) {
                    scan_port(host, port);
                }

                break;
            case 4:
                printf("Enter the IP address to scan: ");
                scanf("%s", host);

                printf("Enter the list of ports (separated by space): ");
                scanf(" %[^\n]", port_List);

                char *token = strtok(port_List, " ");
                while (token != NULL) {
                    int port = atoi(token);
                    scan_port(host, port);
                    token = strtok(NULL, " ");
                }

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
