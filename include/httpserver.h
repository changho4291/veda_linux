#ifndef __HTTPSERVER_H__
#define __HTTPSERVER_H__

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdint.h>
#include <sys/epoll.h>

#define MAX_EVENTS 50

typedef struct {
    int sock, efd;
    pthread_t thread;
    struct sockaddr_in servaddr, cliaddr;
    struct epoll_event ev, events[MAX_EVENTS];
} HttpServer;

typedef struct {
	char* path;
	void (*http_func)(int, void*);
} HTTP_API;

int serverCreate(HttpServer* server);
void serverStart(HttpServer* server);
void serverJoin(HttpServer* server);

#endif // __HTTPSERVER_H__