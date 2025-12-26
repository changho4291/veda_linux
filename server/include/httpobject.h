#ifndef __HTTPOBJECT_H__
#define __HTTPOBJECT_H__

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
#include <errno.h>

#include "linkedList.h"

#define MAX_EVENTS 50

typedef struct {
    int sock, efd;
    pthread_t thread;
    struct sockaddr_in servaddr, cliaddr;
    struct epoll_event ev, events[MAX_EVENTS];
    LinkedList* getApi;
    LinkedList* postApi;
    LinkedList* deleteApi;
} HttpServer;

typedef struct {
    char headOption[100][2000];
    int optSize;
    char body[BUFSIZ];
    int bodySize;
} HttpRequest;

typedef struct HTTP_API {
	const char* path;
	void (*http_func)(int, HttpRequest*, void*);
    void* arg;
} HTTP_API;

#endif // __HTTPOBJECT_H__