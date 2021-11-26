#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <string.h>
#define MAX_BUFFER 1024
#define MAX_MSG 64
enum
{
    ERROR,
    USA,
    VERSARE,
    PRELEVARE,
    SALDO,
    OK
};

unsigned int id_bank_account = 0;

int bank_account[10] = {0};

void versaConto(char *str){
    char value[5];
    for(int i =0; i < 4; i++){
        value[i] = str[i+1];
    }
    value[5] = '\0';

    int amount = atoi(value);
    printf("VALUE: %d\n", amount);
    bank_account[id_bank_account]+=amount;
}

int prelevaConto(char *str){
    char value[5];
    for(int i =0; i < 4; i++){
        value[i] = str[i+1];
    }
    value[5] = '\0';

    int amount = atoi(value);
    printf("VALUE: %d\n", amount);
    bank_account[id_bank_account]-=amount;
    return OK;
}

void setIDAccount(char* buff){
    id_bank_account = (int)(buff[1] - '0');
}

int msgToClient(int response, char * message){
    memset(message, 0, strlen(message));

    if (response == ERROR){
            return sprintf(message, "Error \n");
    }else if(response == USA){
            return sprintf(message, "OPERANDO SUL CONTO %d \n", id_bank_account);
    }else if(response == SALDO){
            return sprintf(message, "IL SALDO DISPONIBILE SUL CONTO %d E' : $ %d \n", id_bank_account, bank_account[id_bank_account]);
    }else if(response == VERSARE){
            return sprintf(message, "IL VERSAMENTO SUL CONTO %d E' ANDATO A BUON FINE! \n", id_bank_account);
    }else if(response==PRELEVARE){
            return sprintf(message, "IL PRELIEVO DAL CONTO %d E' ANDATO A BUON FINE! \n", id_bank_account);
    }else{
        return -1;
    }
}

int actionBank(char *str)
{
    if (strlen(str)-1 == 2){
        if (str[0] == 'U' || str[0] == 'u'){
            setIDAccount(str);
            return USA;
        } 
        else return ERROR;

    }else if(strlen(str)-1 == 5){
        if (str[0] == 'V' || str[0] == 'v') {
            versaConto(str);
            return VERSARE;
        }
        else if (str[0] == 'P' || str[0] == 'p') {
            prelevaConto(str);
            return PRELEVARE;
            }
        else return ERROR;
    }
    else if (strlen(str)-1 == 1 && (str[0] == 'S' || str[0] == 's'))
        return SALDO;
    else
        return ERROR;
}

int main()
{
    printf("Starting Bank Server...\n");

    int socket_fd = socket(PF_INET, SOCK_STREAM, 0);
    if (socket_fd == -1)
    {
        perror("Error on socket():");
        exit(1);
    }

    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(3999);

    int yes = 1;
    if (setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
    {
        perror("setsockopt");
        exit(1);
    }

    int bind_ret = bind(socket_fd, (struct sockaddr *)&address, sizeof(address));
    if (bind_ret == -1)
    {
        perror("Error on bind():");
        exit(1);
    }

    int listen_ret = listen(socket_fd, 2);
    if (listen_ret == -1)
    {
        perror("Error on listen():");
        exit(1);
    }

    while (1)
    {
        int socket_connection = accept(socket_fd, NULL, NULL);

        if (socket_connection == -1)
        {
            perror("Error on accept():");
            exit(1);
        }
        printf("Client connected!\n");
        while (1)
        {
            char buffer[MAX_BUFFER];
            ssize_t total_bytes_read = 0;
            ssize_t current_bytes_read = 0;

            do
            {
                current_bytes_read = read(socket_connection, buffer + total_bytes_read, sizeof(buffer) - total_bytes_read);

                if (current_bytes_read == -1)
                {
                    perror("Erron on read(): ");
                    exit(1);
                }

                total_bytes_read += current_bytes_read;

            } while (buffer[total_bytes_read - 1] != '\n');
            buffer[total_bytes_read - 1] = '\0';
            
            int response = actionBank(buffer);
            char message[MAX_MSG];
            int size = msgToClient(response,message); //fare memcpy() e settare message nei vari dell'if
            write(socket_connection, message, size+1);
            if(response == ERROR){
                close(socket_connection);
                break;
            }
        }
    }
    close(socket_fd);

    return 0;
}