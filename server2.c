#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h> // Incluye winsock2.h en lugar de los encabezados POSIX

#pragma comment(lib, "ws2_32.lib") // Vincula con la biblioteca ws2_32.lib

int main(int argc, char **argv) {
    WSADATA wsaData;
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData); // Inicializa Winsock
    if (result != 0) {
        printf("WSAStartup ha fallado: %d\n", result);
        return 1;
    }

    if (argc != 2) {
        printf("Uso: %s <port>\n", argv[0]);
        WSACleanup(); // Limpia Winsock
        return 0;
    }

    char *ip = "127.0.0.1";
    int port = atoi(argv[1]);

    int sockfd;
    struct sockaddr_in server_addr, client_addr;
    char buffer[1024];
    int addr_size;
    int n;

    sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sockfd == INVALID_SOCKET) {
        perror("[-] Error en socket ");
        WSACleanup(); // Limpia Winsock
        return 1;
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = inet_addr(ip);

    n = bind(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr));
    if (n == SOCKET_ERROR) {
        perror("[-] Error de enlace");
        closesocket(sockfd); // Cierra el socket
        WSACleanup(); // Limpia Winsock
        return 1;
    }

    memset(buffer, 0, sizeof(buffer));
    addr_size = sizeof(client_addr);
    n = recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr*)&client_addr, &addr_size);
    if (n == SOCKET_ERROR) {
        perror("[-] Error de recepción");
        closesocket(sockfd);
        WSACleanup();
        return 1;
    }
    printf("[+] Recepcion de datos: %s\n", buffer);

    memset(buffer, 0, sizeof(buffer));
    strcpy(buffer, "Bienvenidos a nuestro Servidor UDP .");
    n = sendto(sockfd, buffer, strlen(buffer), 0, (struct sockaddr*)&client_addr, addr_size);
    if (n == SOCKET_ERROR) {
        perror("[-]Error de envío");
        closesocket(sockfd);
        WSACleanup();
        return 1;
    }
    printf("[+] Datos enviados: %s\n", buffer);

    // Cerrar el socket y limpiar Winsock
    closesocket(sockfd);
    WSACleanup();
    return 0;
}
