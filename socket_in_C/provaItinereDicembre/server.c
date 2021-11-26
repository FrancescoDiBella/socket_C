// prova in itinere del dicembre 2019/2020 //

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int funcSum(char *buffer)
{
    unsigned end_num_index = -1;
    unsigned start_num_index = 0;
    int sum = 0;
    int valid = 0;

    while ((buffer[start_num_index] < '0' || buffer[start_num_index] > '9') && buffer[start_num_index] != '\n' && buffer[start_num_index] != '\r')
        start_num_index++;

    if (buffer[start_num_index] != '\n' && buffer[start_num_index] != '\r')
        end_num_index = start_num_index;
    else
        return -1;

    while (buffer[end_num_index] >= '0' && buffer[end_num_index] <= '9' && buffer[end_num_index] != '\n' && buffer[end_num_index] != '\r')
    {
        end_num_index++;
    }

    if (buffer[end_num_index] == '\n' || buffer[end_num_index] == '\r')
    {
        for (int j = start_num_index; j < end_num_index; j++)
        {
            sum += (int)(buffer[j] - '0');
        }
        return sum;
    }else return -1;
    
}

int main()
{
    printf("Start Server...\n");

    int socket_fd = socket(PF_INET, SOCK_STREAM, 0);
    int totalSum = 0;

    if (socket_fd == -1)
    {
        perror("Error on socket(): ");
        exit(1);
    }

    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_port = htons(33333);
    address.sin_addr.s_addr = INADDR_ANY;

    int yes = 1;
    if (setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
    {
        perror("setsockopt");
        exit(1);
    }

    int bind_return_code = bind(socket_fd, (struct sockaddr *)&address, sizeof(address));

    if (bind_return_code == -1)
    {
        perror("Error on bind(): ");
        exit(1);
    }

    int listen_return_code = listen(socket_fd, 1);

    if (listen_return_code == -1)
    {
        perror("Error on listen(): ");
        exit(1);
    }

    /*
    struct sockaddr_in address_client;
    socklen_t address_client_len = sizeof(address_client);

    int socket_connection = accept(socket_fd, (struct sockaddr*) address_client, &address_client_len);
    */
    while (1)
    {
        int socket_connection = accept(socket_fd, NULL, NULL);
        if (socket_connection == -1)
        {
            perror("Error on accept(): ");
            exit(1);
        }

        printf("Client Connected!\n");

        ssize_t MAX_BUFFER = 1024;
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

        int partial_sum = funcSum(buffer);

        if(partial_sum != -1){
            totalSum += partial_sum;
            sprintf(buffer, "Total sum: %d\n", totalSum);
        }else{
            sprintf(buffer, "Error on input\n");
        }
        
        printf("TotalSum: %d\nPartialSum: %d\n", totalSum, partial_sum);

        int write_ret_code = write(socket_connection, buffer, strlen(buffer));
        if(write_ret_code == -1){
            perror("Error on write(): ");
            exit(1);
        }

        close(socket_connection);
    }

    return 0;
}
