#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>

// ANSI escape codes for text colors
#define GREEN   "\x1b[32m"
#define RED     "\x1b[31m"
#define CYAN    "\x1b[36m"
#define MAGENTA "\x1b[35m"
#define YELLOW  "\x1b[33m"
#define BLUE    "\x1b[34m"
#define RESET   "\x1b[0m"



int validate_ip_address(const char ip_address[]) {
    struct sockaddr_in sa;
    int result = inet_pton(AF_INET, ip_address, &(sa.sin_addr));
    return result != 0;
}

int is_number(const char str[]) {
    int i = 0;
    while (str[i]) {
        if (!isdigit(str[i])) {
            return 0;
        }
        i++;
    }
    return 1;
}

int scan_port(const char host[], int port) {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        perror("socket");
        return -1;
    }

    struct sockaddr_in server = {0};
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr(host);
    server.sin_port = htons(port);

    int result = connect(sock, (struct sockaddr*)&server, sizeof(server));

    close(sock);

    return result;
}

int main() {
    char host[100];
    char portInput[100];
    int choice, first_port, endPort;
    char port_List[100];
    char *token;
    char input_choice[10];

    while (1) {
        printf("\n\n\n");
                printf(CYAN "+=======================================+\n");
        printf("|    -- -- Simple Port Scanner -- --    " CYAN "|\n");
        printf("+=======================================+\n");
        printf( CYAN "|" YELLOW " 1. Scan all ports                     " CYAN "|\n");
        printf( CYAN "|" YELLOW " 2. Scan a specific port               " CYAN "|\n");
        printf( CYAN "|" YELLOW " 3. Scan a range of ports              " CYAN "|\n");
        printf( CYAN "|" YELLOW " 4. Scan multiple ports                " CYAN "|\n");
        printf( CYAN "|" YELLOW " 5. Exit                               " CYAN "|\n");
        printf("+=======================================+\n");
        printf( GREEN "Enter your choice: " );
        scanf("%s", input_choice);

        // Filter input to allow only numbers from 1 to 5
        if (strlen(input_choice) != 1 || input_choice[0] < '1' || input_choice[0] > '5') {
            printf( RED "Invalid choice. Please try again.\n" );
            while (getchar() != '\n'); // Clear input buffer
            continue;
        }

        choice = input_choice[0] - '0';
        switch (choice) {
            case 1:
                printf( BLUE "Enter the IP address to scan: ");
                scanf("%s", host);
                printf("\n");
                if (!validate_ip_address(host)) {
                    printf( RED "Invalid IP address. Please try again.\n");
                    break;
                }
				printf( CYAN "\n- - - - - - - - - - - - - - - - - - - - -\n");
                for (int port = 1; port <= 65535; port++) {
                    int result = scan_port(host, port);
                    if (result == 0) {
                        printf( GREEN "Port %d is open\n", port);
                    }
                }
                printf( CYAN "- - - - - - - - - - - - - - - - - - - - -\n");
                printf( GREEN "\nScan completed.\n");

                break;

            case 2:
                printf( BLUE "Enter the IP address to scan: ");
                scanf("%s", host);

                if (!validate_ip_address(host)) {
                    printf( RED "Invalid IP address. Please try again.\n");
                    while (getchar() != '\n'); // Clear input buffer
                    break;
                }

                printf("Enter the port to scan: ");
                scanf(" %[^\n]", portInput);
                printf("\n");
                printf( CYAN "- - - - - - - - - - - - - - - - - - - - -\n");
                token = strtok(portInput, " ");

                while (token != NULL) {
                    if (strpbrk(token, " \t")) {
                        printf( RED "Invalid port number: %s\n", token);
                    } else if (!is_number(token)) {
                        printf( RED "Invalid port number: %s\n", token);
                    } else {
                        first_port = atoi(token);
                        int result = scan_port(host, first_port);
                        if (result == 0) {
                            printf( GREEN "Port %d is open\n", first_port);
                        } else {
                            printf( MAGENTA "Port %d is closed\n", first_port);
                        }
                    }
                    token = strtok(NULL, " ");
                }
                
                                printf( CYAN "- - - - - - - - - - - - - - - - - - - - -\n");
                printf( GREEN "\nScan completed.\n");
                break;

            case 3:
                printf( BLUE "Enter the IP address to scan: ");
                scanf("%s", host);

                if (!validate_ip_address(host)) {
                    printf( RED "Invalid IP address. Please try again.\n");
                    break;
                }

                printf("Enter the starting port of the range: ");
                scanf("%d", &first_port);

                printf("Enter the ending port of the range: ");
                scanf("%d", &endPort);
                printf( CYAN "\n- - - - - - - - - - - - - - - - - - - - -\n");
                for (int port = first_port; port <= endPort; port++) {
                    int result = scan_port(host, port);
                    if (result == 0) {
                        printf( GREEN "Port %d is open\n", port);
                    }
                }
                                printf( CYAN "- - - - - - - - - - - - - - - - - - - - -\n");
                printf( GREEN "\nScan completed.\n");

                break;

            case 4:
                printf( BLUE "Enter the IP address to scan: ");
                scanf("%s", host);

                if (!validate_ip_address(host)) {
                    printf( RED "Invalid IP address. Please try again.\n");
                    break;
                }

                printf("Enter the list of ports (separated by space): ");
                scanf(" %[^\n]", port_List);
                printf("\n");
                int port_count = 0;
                int ports[100];

                token = strtok(port_List, " ");

                while (token != NULL) {
                    if (!is_number(token)) {
                        printf( RED "Invalid port number: %s\n", token);
                    } else {
                        ports[port_count++] = atoi(token);
                    }
                    token = strtok(NULL, " ");
                }
                                printf( CYAN "\n- - - - - - - - - - - - - - - - - - - - -\n");
                for (int i = 0; i < port_count; i++) {
                    int result = scan_port(host, ports[i]);
                    if (result == 0) {
                        printf( GREEN "Port %d is open\n", ports[i]);
                    } else {
                        printf( MAGENTA "Port %d is closed\n", ports[i]);
                    }
                }
                                printf( CYAN "- - - - - - - - - - - - - - - - - - - - -\n");
                printf( GREEN "\nScan completed.\n");

                break;

            case 5:
                printf( RED "Exiting Port Scanner . . .\n");
                exit(0);

            default:
                printf( RED "Invalid choice. Please try again.\n");
        }
    }

    return 0;
}
