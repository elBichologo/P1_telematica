#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> // Incluye encabezados POSIX para sockets
#include <sys/socket.h>
#include <netinet/in.h>

int main(int argc, char **argv) {
    int sockfd;
    struct sockaddr_in server_addr, client_addr;
    int port;
    char buffer[1024];
    socklen_t addr_size;
    int n;

    if (argc != 2) {
        printf("Uso: %s <port>\n", argv[0]);
        return 1;
    }

    port = atoi(argv[1]);

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("[-] Error en socket ");
        return 1;
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    n = bind(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr));
    if (n < 0) {
        perror("[-] Error de enlace");
        close(sockfd);
        return 1;
    }

    memset(buffer, 0, sizeof(buffer));
    addr_size = sizeof(client_addr);
    n = recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr*)&client_addr, &addr_size);
    if (n < 0) {
        perror("[-] Error de recepción");
        close(sockfd);
        return 1;
    }
    printf("[+] Recepcion de datos: %s\n", buffer);

    memset(buffer, 0, sizeof(buffer));
    strcpy(buffer, "Bienvenidos a nuestro Servidor UDP .");
    n = sendto(sockfd, buffer, strlen(buffer), 0, (struct sockaddr*)&client_addr, addr_size);
    if (n < 0) {
        perror("[-]Error de envío");
        close(sockfd);
        return 1;
    }
    printf("[+] Datos enviados: %s\n", buffer);

    // Cerrar el socket
    close(sockfd);
    return 0;
}

