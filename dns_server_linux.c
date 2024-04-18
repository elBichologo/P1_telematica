#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define MAX_RECORDS 100
#define BUFFER_SIZE 1024
#define PORT 5300 // Puerto en el que el servidor escuchará peticiones

// Estructura de registro DNS
struct dns_record {
    char domain[256];
    char ip[16];
};

// Estructura que contiene la tabla DNS
struct dns_record dns_table[MAX_RECORDS];
int num_records = 0;

// Función para cargar registros DNS desde un archivo
void load_dns_records(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Error al abrir el archivo de registros DNS");
        exit(EXIT_FAILURE);
    }

    while (fscanf(file, "%s %s", dns_table[num_records].domain, dns_table[num_records].ip) != EOF) {
        num_records++;
        if (num_records >= MAX_RECORDS) {
            fprintf(stderr, "Número máximo de registros DNS alcanzado\n");
            break;
        }
    }

    fclose(file);
}

// Función para manejar las solicitudes DNS
void handle_dns_request(int client_socket, struct sockaddr_in *client_addr, socklen_t client_addr_len) {
    char buffer[BUFFER_SIZE];
    int recv_len = recvfrom(client_socket, buffer, BUFFER_SIZE, 0, (struct sockaddr *)client_addr, &client_addr_len);

    if (recv_len < 0) {
        perror("Error al recibir datos");
        return;
    }

    buffer[recv_len] = '\0'; // Añadir terminador de cadena

    // Buscar la respuesta en la tabla DNS
    char response[256] = "No se encontró la solicitud";
    for (int i = 0; i < num_records; i++) {
        if (strcmp(buffer, dns_table[i].domain) == 0) {
            strncpy(response, dns_table[i].ip, sizeof(response));
            break;
        } else if (strcmp(buffer, dns_table[i].ip) == 0) {
            strncpy(response, dns_table[i].domain, sizeof(response));
            break;
        }
    }

    // Enviar la respuesta al cliente
    sendto(client_socket, response, strlen(response), 0, (struct sockaddr *)client_addr, client_addr_len);
}

// Función principal del servidor DNS
int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Uso: %s <archivo_de_registros_DNS>\n", argv[0]);
        return EXIT_FAILURE;
    }

    // Cargar los registros DNS desde el archivo proporcionado
    load_dns_records(argv[1]);

    // Crear el socket UDP
    int server_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if (server_socket < 0) {
        perror("Error al crear el socket");
        return EXIT_FAILURE;
    }

    // Configurar la dirección del servidor
    struct sockaddr_in server_addr = {0};
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // Vincular el socket a la dirección del servidor
    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Error al vincular el socket");
        close(server_socket);
        return EXIT_FAILURE;
    }

    printf("Servidor DNS escuchando en el puerto %d...\n", PORT);

    // Manejar las solicitudes DNS entrantes
    struct sockaddr_in client_addr;
    socklen_t client_addr_len = sizeof(client_addr);

    while (1) {
        handle_dns_request(server_socket, &client_addr, client_addr_len);
    }

    // Cierre del socket
    close(server_socket);

    return EXIT_SUCCESS;
}
