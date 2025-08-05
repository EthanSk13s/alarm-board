#ifndef NET_H
#define NET_H

#include <curl/curl.h>

#include "../utils.h"

#define NET_MAX_URL_LENGTH 250

typedef struct
{
    CURL* curl;
    Buffer buf;
    char url[NET_MAX_URL_LENGTH];
} NetworkHandler;

int net_init_handler(NetworkHandler* net_handler, char* url);
void net_free_handler(NetworkHandler* net_handler);
#endif