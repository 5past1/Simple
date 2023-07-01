#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

int validate_ip_address(const char ip_address[]) {
    struct sockaddr_in sa;
    int result = inet_pton(AF_INET, ip_address, &(sa.sin_addr));
    return result != 0;
}

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

    if (connect(sock, (struct sockaddr*)&server, sizeof(server)) == 0) {
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

                if (!validate_ip_address(host)) {
                    printf("Invalid IP address. Please try again.\n");
                    break;
                }

                for (int port = 1; port <= 65535; port++) {
                    scan_port(host, port);
                }

                printf("Scan completed.\n");

                break;
            case 2:
                printf("Enter the IP address to scan: ");
                scanf("%s", host);

                if (!validate_ip_address(host)) {
                    printf("Invalid IP address. Please try again.\n");
                    break;
                }

                printf("Enter the port to scan: ");
                scanf("%d", &first_port);

                scan_port(host, first_port);
                printf("Scan completed.\n");

                break;
            case 3:
                printf("Enter the IP address to scan: ");
                scanf("%s", host);

                if (!validate_ip_address(host)) {
                    printf("Invalid IP address. Please try again.\n");
                    break;
                }

                printf("Enter the starting port of the range: ");
                scanf("%d", &first_port);

                printf("Enter the ending port of the range: ");
                scanf("%d", &endPort);

                for (int port = first_port; port <= endPort; port++) {
                    scan_port(host, port);
                }

                printf("Scan completed.\n");

                break;
            case 4:
                printf("Enter the IP address to scan: ");
                scanf("%s", host);

                if (!validate_ip_address(host)) {
                    printf("Invalid IP address. Please try again.\n");
                    break;
                }

                printf("Enter the list of ports (separated by space): ");
                scanf(" %[^\n]", port_List);

                int port_count = 0;
                int ports[100];

                char *token;
                token = strtok(port_List, " ");
                while (token != NULL) {
                    ports[port_count++] = atoi(token);
                    token = strtok(NULL, " ");
                }

                for (int i = 0; i < port_count; i++) {
                    scan_port(host, ports[i]);
                }

                printf("Scan completed.\n");

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
