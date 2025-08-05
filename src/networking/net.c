#include <string.h>
#include <stdlib.h>

#include "net.h"

#define MAX_BUFFER_SIZE 5000

int net_init_handler(NetworkHandler* net_handler, char* url)
{
    net_handler->curl = curl_easy_init();
    if (!net_handler->curl)
    {
        return -1;
    }

    int url_len = strlen(url);
    if (url_len > NET_MAX_URL_LENGTH)
    {
        return -1;
    }


    net_handler->buf.buffer = malloc(MAX_BUFFER_SIZE);
    if (!net_handler->buf.buffer)
    {
        return -1;
    }

    net_handler->buf.max_length = MAX_BUFFER_SIZE;
    net_handler->buf.buf_length = 0;

    strncpy(net_handler->url, url, NET_MAX_URL_LENGTH);
    curl_easy_setopt(net_handler->curl, CURLOPT_URL, net_handler->url);

    return 0;
}

void net_free_handler(NetworkHandler* net_handler)
{
    curl_easy_cleanup(net_handler->curl);
}