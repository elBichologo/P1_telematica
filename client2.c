#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h> // Incluye winsock2.h en lugar de los encabezados POSIX

// Vincula con la biblioteca ws2_32.lib
#pragma comment(lib, "ws2_32.lib")

int main(int argc, char **argv) {
    WSADATA wsaData;
    SOCKET sockfd;
    struct sockaddr_in server_addr, client_addr;
    int port;
    char buffer[1024];
    int addr_size;

    // Inicializar Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        fprintf(stderr, "Error al inicializar Winsock: %d\n", WSAGetLastError());
        return 1;
    }

    // Verificar el número de argumentos
    if (argc != 2) {
        fprintf(stderr, "Uso: %s <puerto>\n", argv[0]);
        WSACleanup(); // Limpia Winsock
        return 1;
    }

    // Convertir el argumento del puerto a entero
    port = atoi(argv[1]);

    // Crear un socket UDP
    sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sockfd == INVALID_SOCKET) {
        fprintf(stderr, "Error al crear socket: %ld\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }

    // Configurar la dirección del servidor
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    // Enviar mensaje al servidor
    strcpy(buffer, "Hello, World!");
    if (sendto(sockfd, buffer, strlen(buffer), 0, (struct sockaddr *)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        fprintf(stderr, "Error al enviar mensaje: %ld\n", WSAGetLastError());
        closesocket(sockfd);
        WSACleanup();
        return 1;
    }
    printf("[+] Mensaje enviado: %s\n", buffer);

    // Recibir respuesta del servidor
    addr_size = sizeof(client_addr);
    memset(buffer, 0, sizeof(buffer));
    if (recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr *)&client_addr, &addr_size) == SOCKET_ERROR) {
        fprintf(stderr, "Error al recibir respuesta: %ld\n", WSAGetLastError());
        closesocket(sockfd);
        WSACleanup();
        return 1;
    }
    printf("[+] Respuesta recibida: %s\n", buffer);

    // Cerrar el socket
    closesocket(sockfd);

    // Limpiar Winsock
    WSACleanup();

    return 0;
}

