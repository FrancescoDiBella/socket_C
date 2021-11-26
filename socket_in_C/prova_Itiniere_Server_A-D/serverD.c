//prova in itinere del 18/12/2019
//Server che recepisce, controlla e rispedisce un buffer di char
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int digitToInt(char digit){
    return (int)(digit - '0');
}

int MUL(char* str){
    int prod = 1;

    size_t i = 0;

    while(str[i] >= '0' && str[i] <= '9'){
        prod*= digitToInt(str[i]);
        ++i;
    }

    return prod;
}

int main(){
    printf("Hello World! I'm a server on port 7777\n");
    struct sockaddr_in address;

    /*inizializzazione della struct sockaddr_in */
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(7777);
    address.sin_family = AF_INET;

    // creazione address_len e inizializzazione della socket tramite socket()
    socklen_t address_len = sizeof(address); //opzionale
    int server_fd = socket(AF_INET, SOCK_STREAM, 0); 

    //controllo del valore di ritorno di socket
    if (server_fd == -1){
        perror("Error on socket initializiation:");
        exit(1);
    }

    //bind() sulla socket e sulla porta descritta in address nella
    
    int return_code_bind = bind(server_fd, (struct sockaddr *) &address, address_len);
    if(return_code_bind == -1){
        perror("Error on bind():");
        exit(1);
    }

    int return_code_listen = listen(server_fd, 1);
    if(return_code_listen == -1){
        perror("Error on listen():");
        exit(1);
    }

    struct sockaddr_in client_address;
    socklen_t client_address_len = sizeof(client_address);

    int client_fd = accept(server_fd, (struct sockaddr *) &client_address, &client_address_len);
    if(client_fd == -1){
        perror("Error on accept():");
        exit(1);
    }
    printf("Client connected!\n");

    size_t buffer_size = 1024; 
    char buffer[buffer_size];
    size_t bytes_read = 0;
    size_t totalBytes = 0;

    for(totalBytes = 0; totalBytes < buffer_size-1;){
        bytes_read = read(client_fd, 
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
    write(client_fd, res_MUL_str, res_MUL_strlen);

    printf("Buffer sent to client.\n");

    /* Closing connection */
    printf("Closing.\n");
    shutdown(client_fd, 2);
    shutdown(server_fd, 2);

    close(client_fd);
    close(server_fd);
    return 0;
}