#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <string.h>
#include "offertaUtente.h"

ssize_t MAX_BUFFER = 1024;

int main(){
    printf("Starting N E T S I X \n");

    int socket_fd = socket(PF_INET, SOCK_STREAM, 0);
    if(socket_fd == -1){
        perror("Errore nella creazione della socket:");
        exit(1);
    }

    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(7777);

    int yes = 1;
    if (setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
    {
        perror("setsockopt");
        exit(1);
    }

    int bind_ret  = bind(socket_fd, (struct sockaddr*) &address, sizeof(address));
    if(bind_ret == -1){
        perror("Error on bind:");
        exit(1);
    }

    int return_code_listen = listen(socket_fd, 3);
    if(return_code_listen == -1){
        perror("Error on listen():");
        exit(1);
    }
    
    printf("Waiting for client connection...\n");
    
    while(1){
        char buffer[MAX_BUFFER];
        int client_fd = accept(socket_fd, NULL, NULL);
        if(client_fd == -1) {
            perror("Error on accept:");
            exit(1);
        } 

        printf("\n-----------New Connection Enstablished!-----------\n");

        ssize_t bytes_read = read(client_fd, buffer, sizeof(buffer));
        if(bytes_read == -1){
            perror("Error on read");
            exit(1);
        }
        char *response = episodioCheck(buffer);
        int response_len = strlen(response);
        write(client_fd, response, response_len);
        close(client_fd);
    }
    
    close(socket_fd); 
    return 0;

}