#include "connection.h"

#define SERVER_PORT 8080

int main() {
    // создаем сокет
    int client_socket;
    client_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_SCTP);

    if (client_socket < 0)
    {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in server_addr;
    uint8_t *request;
    ssize_t request_len;
    uint8_t response[max_size_buf];
    ssize_t bytes_received;

    RRCConnectionRequestCoder(&request, &request_len);
        
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    server_addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK); 

    if (connect(client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) 
    {
        perror("connect");
        close(client_socket);
        exit(EXIT_FAILURE);
    }

    if (sctp_sendmsg(client_socket, (void *)request, request_len, NULL, 0, 0, 0, 0, 0, 0) == -1) 
    {
        perror("sctp_sendmsg");
        close(client_socket);
        exit(EXIT_FAILURE);
    }

    bytes_received = sctp_recvmsg(client_socket, response, max_size_buf, NULL, 0, NULL, NULL);

    if (bytes_received > 0) 
    {
        RRCConnectionSetup_t *rrc_response = NULL;

        if(RRCConnectionCompleteDecoder(rrc_response, response, bytes_received))
        {
            printf("RRC Connection Setup decoded\n");
        }
        else
        {
            perror("RRC connection complete decode");
        }
    }

    close(client_socket);

    return 0;
}