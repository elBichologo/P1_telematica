#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> // Incluye encabezados POSIX para sockets
#include <sys/socket.h>
#include <netinet/in.h>

int main(int argc, char **argv) {
    int sockfd;
    struct sockaddr_in server_addr;
    int port;
    char buffer[1024];
    socklen_t addr_size;

    // Verificar el número de argumentos
    if (argc != 2) {
        fprintf(stderr, "Uso: %s <puerto>\n", argv[0]);
        return 1;
    }

    // Convertir el argumento del puerto a entero
    port = atoi(argv[1]);

    // Crear un socket UDP
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("Error al crear socket");
        return 1;
    }

    // Configurar la dirección del servidor
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK); // Usar la dirección de loopback (localhost)

    // Enviar mensaje al servidor
    strcpy(buffer, "Hello, World!");
    if (sendto(sockfd, buffer, strlen(buffer), 0, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Error al enviar mensaje");
        close(sockfd);
        return 1;
    }
    printf("[+] Mensaje enviado: %s\n", buffer);

    // Recibir respuesta del servidor
    addr_size = sizeof(server_addr);
    memset(buffer, 0, sizeof(buffer));
    if (recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr *)&server_addr, &addr_size) < 0) {
        perror("Error al recibir respuesta");
        close(sockfd);
        return 1;
    }
    printf("[+] Respuesta recibida: %s\n", buffer);

    // Cerrar el socket
    close(sockfd);

    return 0;
}