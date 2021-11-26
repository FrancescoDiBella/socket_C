#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

#define NICK_LEN 128
#define BUFFER_LEN 1024

char* get_string(char *buffer){
    for(int i = 0; buffer[i] != '\0'; i++){
        buffer[i] = rand() % 102;
    }
}

int main(){
    srand(time(NULL));
    int run = 1;
    char nick_name_client[NICK_LEN];

    int server_fd = socket(PF_INET, SOCK_STREAM, 0);
    if(server_fd == -1){
        perror("error on socket() ");
        exit(0);
    }

    struct sockaddr_in address;

    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_family = AF_INET;
    address.sin_port = htons(9899);

    int bind_ret_code;
    if((bind_ret_code = bind(server_fd, (struct sockaddr*)&address, sizeof(address))) == -1){
        perror("error on bind(): ");
        exit(0);
    }

    int listen_ret_code;
    if((listen_ret_code = listen(server_fd, 1)) == -1){
        perror("error on listen(): ");
        exit(0);
    }
    char buffer[BUFFER_LEN];
    int client_fd;
    int total_bytes;
    int bytes_read;

    if((client_fd = accept(server_fd, NULL, NULL)) == -1){
            perror("error on accept(): ");
            exit(0);
    }
    printf("Client connected\n");

    while(run){
        bytes_read = 0;//total_bytes = 

        if(run == 1){
            sprintf(buffer, "Ciao! Sono serverRobot, tu come ti chiami? ['bye' per terminare] ...\n");
            write(client_fd, buffer, strlen(buffer));

            //mantenere il nome dell'utente in una variabile;
            for(total_bytes = 0; total_bytes < NICK_LEN - 1;){
                if((bytes_read = read(client_fd, 
                                        nick_name_client + total_bytes,
                                        NICK_LEN - total_bytes)) == -1){
                    perror("error on read(): ");
                    exit(0);
                }

                total_bytes+=bytes_read;
                if(nick_name_client[total_bytes-1] == '\n'){
                    nick_name_client[total_bytes-1] = '\0';
                    break;
                }
                    
            }
            bytes_read = 0;
            run++;
        }

            sprintf(buffer, "%s! Prego, digita quello che vuoi!: \n", nick_name_client);
            write(client_fd, buffer, strlen(buffer));

            //mantenere il nome dell'utente in una variabile;
            for(total_bytes = 0; total_bytes < BUFFER_LEN - 1;){
                if((bytes_read = read(client_fd, 
                                        buffer + total_bytes,
                                        BUFFER_LEN - total_bytes)) == -1){
                    perror("error on read(): ");
                    exit(0);
                }

                total_bytes+=bytes_read;
                if(buffer[total_bytes-1] == '\n')
                    break;
            }

            if(buffer=="bye\n"){
                run = 0;
                break;
            }

            sprintf(buffer, "Da serverRobot: %s\n", "get_string(buffer)");
            write(client_fd, buffer, strlen(buffer));

    }

    close(server_fd);
    close(client_fd);
    




    return 0;
}