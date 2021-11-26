#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <string.h>

ssize_t MAX_BUFFER = 1024;
#define MAX_LINE_LENGTH 128


ssize_t readLine(int descriptor, char* line_buffer){
    char *write_line_slot = line_buffer; //&line_buffer[0]
    ssize_t total_bytes_read = 0;

    while (1){
            ssize_t bytes_read = read(descriptor, write_line_slot, sizeof(char));
            if (bytes_read == -1){
                perror("Error on read");
                exit(1);
            }

            if(bytes_read == 0){
                break;
            }

            total_bytes_read += bytes_read;

            int is_line_ended = (write_line_slot[0] == '\n') && (write_line_slot[-1] == '\r');

            ++write_line_slot;

            if(is_line_ended){
                *write_line_slot = '\0';
                break;
            }
        }
    return total_bytes_read;
}

int main()
{
    printf("Starting N E T S I X \n");

    int socket_fd = socket(PF_INET, SOCK_STREAM, 0);
    if (socket_fd == -1)
    {
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

    int bind_ret = bind(socket_fd, (struct sockaddr *)&address, sizeof(address));
    if (bind_ret == -1)
    {
        perror("Error on bind:");
        exit(1);
    }

    int return_code_listen = listen(socket_fd, 3);
    if (return_code_listen == -1)
    {
        perror("Error on listen():");
        exit(1);
    }

    
    char line_buffer[MAX_LINE_LENGTH];

    while (1)
    {
        printf("Waiting for client connection...\n");
        char buffer[MAX_BUFFER];
        int socket_connection = accept(socket_fd, NULL, NULL);
        if (socket_connection == -1)
        {
            perror("Error on accept:");
            exit(1);
        }

        printf("\n-----------New Connection Enstablished!-----------\n");

        readLine(socket_connection, line_buffer);
        printf("Request line: %s", line_buffer);

        int reading_headers = 1;
        while(reading_headers){
            ssize_t line_length = readLine(socket_connection, line_buffer);
            if(line_length == 2)
                break;
            
            printf("Header : %s", line_buffer);
        }
        
        printf("Header finisched, waiting for body\n");

        ssize_t body_length = readLine(socket_connection, line_buffer);
        if(body_length > 2)
            printf("Request body : %s", line_buffer);
        else printf("The request didn't contain any body\n");
        /*        
        write(socket_connection, line_buffer, size);*/
        close(socket_connection);
    }
    close(socket_fd);
    return 0;
}