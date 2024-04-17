# Proyecto de Cliente-Servidor

Este proyecto contiene código para un cliente y un servidor que se comunican mediante UDP. A continuación, se explican los pasos para compilar y ejecutar ambos programas.

## Requisitos

- GCC para compilar los programas.
- Biblioteca `ws2_32` para manejar la red en Windows.
- Línea de comandos (cmd) para ejecutar los programas.

## Compilación

Para compilar el cliente y el servidor, utiliza los siguientes comandos en la línea de comandos:

- Compilar el cliente:

    ```shell
    gcc client2.c -o client2 -lws2_32
    ```

- Compilar el servidor:

    ```shell
    gcc server2.c -o server2 -lws2_32
    ```

## Ejecución

Sigue estos pasos para ejecutar el cliente y el servidor:

1. **Inicia el servidor:**
    
    Primero, inicia el servidor en la línea de comandos usando el siguiente comando:

    ```shell
    ./server2 4455
    ```

    Esto iniciará el servidor en el puerto 4455. Asegúrate de que el servidor esté en ejecución antes de iniciar el cliente.

2. **Inicia el cliente:**
    
    En una nueva ventana de la línea de comandos, inicia el cliente usando el siguiente comando:

    ```shell
    ./client2 4455
    ```

    Esto conectará el cliente al servidor a través del puerto 4455.

## Notas

- Asegúrate de que tanto el cliente como el servidor estén ejecutándose en la misma máquina o en máquinas en la misma red local.
- Si cambias el puerto, asegúrate de actualizar tanto el comando de inicio del servidor como el del cliente con el nuevo número de puerto.

## Problemas conocidos

Si experimentas problemas al compilar o ejecutar los programas, verifica que tienes instalada la biblioteca `ws2_32` y que estás utilizando una versión compatible de GCC.

---
