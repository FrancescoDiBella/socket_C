//prova in itinere del 18/12/2019

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main(){
    int socket_fd;
    int return_code_bind;
    printf("Hello World! I'm a server on port 7777\n");
    struct sockaddr_in address;

    /*inizializzazione della struct sockaddr_in */
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(7777); 
    address.sin_family = AF_INET; //socket Internet

    // creazione address_len e inizializzazione della socket tramite socket()
    socklen_t address_len = sizeof(address);
    //controllo del valore di ritorno di socket
    if ((socket_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1){
        perror("Error on socket initializiation:");
        exit(1);
    }

    //bind() sulla socket e sulla porta descritta in address
    if((return_code_bind = bind(socket_fd, (struct sockaddr *) &address, sizeof(address))) == -1){
        perror("Error on bind():");
        exit(1);
    }

    int return_code_listen = listen(socket_fd, 1);
    if(return_code_listen == -1){
        perror("Error on listen():");
        exit(1);
    }

    struct sockaddr_in client_socket;
    socklen_t client_len = sizeof(client_socket);

    int client_fd = accept(socket_fd, (struct sockaddr *) &client_socket, &client_len); // accept(socket_fd, NULL, NULL); 
    if(client_fd == -1){
        perror("Error on accept():");
        exit(1);
    }
    printf("Client connected!\n");

    size_t buffer_size = 1024; 
    char buffer[buffer_size];
    size_t bytes_read = 0;
    size_t totalBytes = 0;

    for(; totalBytes < buffer_size-1;){ // 1254\n\0

        bytes_read = read(client_fd, 
                            buffer + totalBytes,
                            buffer_size - totalBytes);
        
        printf("Bytes read: %ld\n", bytes_read);

        if(bytes_read == -1){
            perror("Error on read()");
            exit(1);
        }
        totalBytes+=bytes_read;

        if(buffer[totalBytes-1] == '\n')
          break;
    }

    size_t newLineIndex = totalBytes-2; //index del carattere \n
    buffer[newLineIndex] = '\0';

    if(buffer[newLineIndex] == '\n'){
        fprintf(stderr,"Expected newline at index %lu ('%c')", newLineIndex, buffer[newLineIndex]);
    }
    

    for(int i=0; i < strlen(buffer); i++){
        if(buffer[i] <48 || buffer[i] >57){
            fprintf(stderr,"Invalid char at index %d ('%c')", i, buffer[i]);
            fprintf(stderr,"Client in ascolto dalla porta %d\n", (int)client_socket.sin_port);
            exit(1);
        }
        
    }
    
    write(STDOUT_FILENO, buffer, strlen(buffer));

    printf("Closing.\n");
    shutdown(client_fd, 2);
    shutdown(socket_fd, 2);

    close(client_fd);
    close(socket_fd);
    return 0;
}