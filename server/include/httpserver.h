#ifndef __HTTPSERVER_H__
#define __HTTPSERVER_H__

#include "httpobject.h"

// HTTP 기본 헤더
#define HTTP_DEF_HEAD "HTTP/1.1 200 OK\r\nContent-Type: application/json\r\nConnection: close\r\n"

/**
 * @brief HTTP 서버 초기화 (생성자)
 * 
 * @param server    서버 객체
 * @return int      생성 결과
 */
int serverCreate(HttpServer* server);

/**
 * @brief HTTP 서버 스레드 실행
 * 
 * @param server    서버 객체
 */
void serverStart(HttpServer* server);

/**
 * @brief HTTP 서버 스레드 조인
 * 
 * @param server    서버 객체
 */
void serverJoin(HttpServer* server);

/**
 * @brief HTTP GET 컨테이너 추가 함수
 * 
 * @param server    서버 객체
 * @param path      url 경로
 * @param http_func 수행할 기능
 * @param arg       수행할 객체
 */
void setGetApi(HttpServer* server, const char* path
    , void (*http_func)(int, HttpRequest*, void*), void* arg);

/**
 * @brief HTTP POST 컨테이너 추가 함수
 * 
 * @param server    서버 객체
 * @param path      url 경로
 * @param http_func 수행할 기능
 * @param arg       수행할 객체
 */
void setPostApi(HttpServer* server, const char* path
    , void (*http_func)(int, HttpRequest*, void*), void* arg);

/**
 * @brief HTTP DELETE 컨테이너 추가 함수
 * 
 * @param server    서버 객체
 * @param path      url 경로
 * @param http_func 수행할 기능
 * @param arg       수행할 객체
 */
void setDeleteApi(HttpServer* server, const char* path
    , void (*http_func)(int, HttpRequest*, void*), void* arg);

#endif // __HTTPSERVER_H__