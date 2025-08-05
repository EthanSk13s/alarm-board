#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "net.h"

#define INITIAL_BUFFER_SIZE 5000

static size_t write_callback(char* data, size_t size, size_t nmemb, void* user_data)
{
    size_t realsize = size * nmemb;
    Buffer* buffer = (Buffer*) user_data;

    if (buffer->max_length < buffer->buf_length + realsize)
    {
        char* new_buf = realloc(buffer->buffer, buffer->buf_length + realsize + 1);
        if (!new_buf)
        {
            return 0;
        }

        buffer->buffer = new_buf;
        buffer->max_length = buffer->buf_length + realsize + 1;
    }

    memcpy(&buffer->buffer[buffer->buf_length], data, realsize);
    buffer->buf_length += realsize;
    buffer->buffer[buffer->buf_length] = 0;

    return realsize;
}

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


    net_handler->buf.buffer = malloc(INITIAL_BUFFER_SIZE);
    if (!net_handler->buf.buffer)
    {
        return -1;
    }

    net_handler->buf.max_length = INITIAL_BUFFER_SIZE;
    net_handler->buf.buf_length = 0;

    strncpy(net_handler->url, url, NET_MAX_URL_LENGTH);

    curl_easy_setopt(net_handler->curl, CURLOPT_URL, net_handler->url);
    curl_easy_setopt(net_handler->curl, CURLOPT_WRITEDATA, (void*) &net_handler->buf);
    curl_easy_setopt(net_handler->curl, CURLOPT_WRITEFUNCTION, write_callback);

    return 0;
}

void net_free_handler(NetworkHandler* net_handler)
{
    curl_easy_cleanup(net_handler->curl);

    free(net_handler->buf.buffer);
    net_handler->buf.buffer = NULL;

    net_handler->buf.max_length = 0;
    net_handler->buf.buf_length = 0;
}