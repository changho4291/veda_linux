#ifndef __BUZZCONTROLLER_H__
#define __BUZZCONTROLLER_H__

#include <stdio.h>
#include <pthread.h>

#include "cJSON.h"

#include "peripheral.h"
#include "httpserver.h"
#include "fnd.h"
#include "buzz.h"

/**
 * @brief BuzzController 정보를 담는 구조체 (객체)
 */
typedef struct {
    HttpServer* sver;   // HTTP 서버 객체 (의존주입)
    Fnd fnd;            // fnd 객체
    Buzz buzz;          // 부저 객체
    pthread_t thread;   // BuzzController 가 관리하는 스레드
} BuzzController;

/**
 * @brief BuzzController 생성자
 * 
 * @param control BuzzController 초기화 구조체
 * @param sv HttpServer (생성 되어 있어야 함)
 */
void buzzControllerCreate(BuzzController* control, HttpServer* sv);

/**
 * @brief FND 제어 POST /fnd
 * 
 * @param csock 클라이언트 소켓
 * @param req   요청 데이터
 * @param arg   BuzzController 객체 수신
 */
void fndSet(int csock, HttpRequest* req, void* arg);

/**
 * @brief FND 종료 DELETE /fnd
 * 
 * @param csock 클라이언트 소켓
 * @param req   요청 데이터
 * @param arg   BuzzController 객체 수신
 */
void fndDelete(int csock, HttpRequest* req, void* arg);

/**
 * @brief 부저 실행 POST /buzz
 * 
 * @param csock 클라이언트 소켓
 * @param req   요청 데이터
 * @param arg   BuzzController 객체 수신
 */
void buzzOn(int csock, HttpRequest* req, void* arg);

/**
 * @brief 부저 종료 DELETE /buzz
 * 
 * @param csock 클라이언트 소켓
 * @param req   요청 데이터
 * @param arg   BuzzController 객체 수신
 */
void buzzOff(int csock, HttpRequest* req, void* arg);

#endif // __BUZZCONTROLLER_H__