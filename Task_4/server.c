#include "connection.h"

#define SERVER_PORT 8080

int main()
{
    int listener_socket;

    listener_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_SCTP);

    if (listener_socket < 0) 
    {
        perror("socket");
        exit(-1);
    }
    
    uint8_t buf[max_size_buf];
    size_t bytes_received;
    uint8_t *response;
    ssize_t response_len;
    
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    // привязываем сокет
    if (bind(listener_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) 
    {
        perror("Error bind");
        close(listener_socket);
        exit(-1);
    }

    // инициализируем слушающий сокет
    if (listen(listener_socket, 1) == -1)
    {
        perror("Error listen");
        close(listener_socket);
        exit(-1);
    }
    
    int sock;

    while (true) 
    {   
        // принимаем входящие соединения
        sock = accept(listener_socket, NULL, NULL);

        if (sock < 0) 
        {
            perror("accept");
            close(listener_socket);
            exit(-1);
        }

        bytes_received = sctp_recvmsg(sock, buf, max_size_buf, NULL, 0, NULL, NULL);

        if (bytes_received > 0)
        {        
            RRCConnectionRequest_t *rrc_con_req = NULL;
            
            if(RRCConnectionRequestDecoder(rrc_con_req, buf, bytes_received))
            {
                printf("The decryption has passed\n");
                RRCConnectionCompleteCoder(&response, &response_len);

                if(sctp_sendmsg(sock, (void *)response, response_len, NULL, 0, 0, 0, 0, 0, 0) > 0)
                {
                    printf("Departure\n");
                }
                else
                {
                    perror("Errors");
                }
            }
            else
            {
                printf("Decoding error.\n");
            }
        }
        close(sock);
    }
    
    close(listener_socket);
    
    return 0;
}