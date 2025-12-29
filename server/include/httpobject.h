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

/**
 * @brief HTTP Server 객체
 * 
 */
typedef struct {
    int sock, efd;      // 서버 소켓, epoll 소켓
    pthread_t thread;   // 서버 스레드
    struct sockaddr_in servaddr, cliaddr;   // 서버 주소, 클라이언트 주소
    struct epoll_event ev, events[MAX_EVENTS];  // epoll 이벤트
    LinkedList* getApi;     // HTTP GET 메소드 컨테이너
    LinkedList* postApi;    // HTTP POST 메소드 컨테이너
    LinkedList* deleteApi;  // HTTP DELETE 메소드 컨테이너
} HttpServer;

/**
 * @brief HTTP Request 데이터 구조
 * 
 */
typedef struct {
    char headOption[100][2000]; // HTTP 헤더 옵션 배열
    int optSize;                // 옵션 갯수
    char body[BUFSIZ];          // body
    int bodySize;               // body 사이즈
} HttpRequest;

/**
 * @brief HTTP 컨테이너 구조체
 * 
 */
typedef struct HTTP_API {
	const char* path;                               // http url
	void (*http_func)(int, HttpRequest*, void*);    // http url과 매핑돠는 구현 로직
    void* arg;                                      // 구현을 수행하는 객체
} HTTP_API;

#endif // __HTTPOBJECT_H__