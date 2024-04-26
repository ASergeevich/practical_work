#ifndef CONNECTION_H
#define CONNECTION_H

#include <RRCConnectionSetup.h>
#include <RRCConnectionRequest.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/sctp.h>
#include <stdbool.h>


static const size_t max_size_buf = 1024;

void RRCConnectionRequestCoder(uint8_t **buffer, ssize_t *length);
bool RRCConnectionRequestDecoder(RRCConnectionRequest_t *rrc_connection_request, uint8_t *buffer, size_t length);

void RRCConnectionCompleteCoder(uint8_t **buffer, ssize_t *length);
bool RRCConnectionCompleteDecoder(RRCConnectionSetup_t *rrc_connection_request, uint8_t *buffer, size_t length);

#endif