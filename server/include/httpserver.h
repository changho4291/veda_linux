#ifndef __HTTPSERVER_H__
#define __HTTPSERVER_H__

#include "httpobject.h"

#define HTTP_DEF_HEAD "HTTP/1.1 200 OK\r\nContent-Type: application/json\r\nConnection: close\r\n"

int serverCreate(HttpServer* server);

void serverStart(HttpServer* server);

void serverJoin(HttpServer* server);

void setGetApi(HttpServer* server, const char* path
    , void (*http_func)(int, HttpRequest*, void*), void* arg);

void setPostApi(HttpServer* server, const char* path
    , void (*http_func)(int, HttpRequest*, void*), void* arg);

void setDeleteApi(HttpServer* server, const char* path
    , void (*http_func)(int, HttpRequest*, void*), void* arg);

#endif // __HTTPSERVER_H__