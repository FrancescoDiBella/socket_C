//prova in itinere del 18/12/2019
//Server che recepisce, controlla e rispedisce un buffer di char
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int MUL(char* str){
    return 0;
}

int main(){
    printf("Hello World! I'm a server on port 7777\n");
    struct sockaddr_in address;

    /*inizializzazione della struct sockaddr_in */
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(7777);
    address.sin_family = AF_INET;

    // creazione address_len e inizializzazione della socket tramite socket()
    socklen_t address_len = sizeof(address);
    int initSocket = socket(AF_INET, SOCK_STREAM, 0);

    //controllo del valore di ritorno di socket
    if (initSocket == -1){
        perror("Error on socket initializiation:");
        exit(1);
    }

    //bind() sulla socket e sulla porta descritta in address nella
    
    int return_code_bind = bind(initSocket, (struct sockaddr *) &address, address_len);
    if(return_code_bind == -1){
        perror("Error on bind():");
        exit(1);
    }

    int return_code_listen = listen(initSocket, 1);
    if(return_code_listen == -1){
        perror("Error on listen():");
        exit(1);
    }

    struct sockaddr_in client_socket;
    socklen_t client_len = sizeof(client_socket);

    int clientDescriptor = accept(initSocket, (struct sockaddr *) &client_socket, &client_len);
    if(clientDescriptor == -1){
        perror("Error on accept():");
        exit(1);
    }
    printf("Client connected!\n");

    size_t buffer_size = 1024; 
    char buffer[buffer_size];
    size_t bytes_read = 0;
    size_t totalBytes = 0;

    for(totalBytes = 0; totalBytes < buffer_size-1;){
        bytes_read = read(clientDescriptor, 
                            buffer + totalBytes, 
                            buffer_size - totalBytes);

        if(bytes_read == -1){
            perror("Error on read()");
            exit(1);
        }
        totalBytes+=bytes_read;
        
        if(buffer[totalBytes-1] == '\n')
            break;
    }

    size_t newLineIndex = totalBytes-2;

    if(buffer[newLineIndex] == '\n'){
        fprintf(stderr,"Expected newline at index %lu ('%c')", newLineIndex, buffer[newLineIndex]);
    }
    

    for(int i=0; i < newLineIndex; i++){
        if(buffer[i] <48 || buffer[i] >57){
            fprintf(stderr,"Invalid char at index %d ('%c')", i, buffer[i]);
            exit(1);
        }
    }


    printf("buffer: %s\n", buffer);

    //Iniviare il risultato di MUL(buffer)

    int res_MUL = MUL(buffer);
    char res_MUL_str[16] ; 
    int res_MUL_strlen = sprintf(res_MUL_str, "%d\n", res_MUL);
    write(clientDescriptor, res_MUL_str, res_MUL_strlen);

    printf("Buffer sent to client.\n");

    /* Closing connection */
    printf("Closing.\n");
    shutdown(clientDescriptor, 2);
    shutdown(initSocket, 2);

    close(clientDescriptor);
    close(initSocket);
    return 0;
}