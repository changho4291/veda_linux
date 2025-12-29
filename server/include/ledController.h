#ifndef __LEDCONTROLLER_H__
#define __LEDCONTROLLER_H__

#include <pthread.h>

#include "cJSON.h"

#include "peripheral.h"
#include "httpserver.h"
#include "led.h"
#include "yl40.h"

#define MAX_LED_NUM 10

/**
 * @brief LedController 정보를 담는 구조체 (객체)
 */
typedef struct {
    HttpServer* sver;       // HTTP 서버 객체 (의존주입)
    Led led;                // led 객체
    YL40 yl40;              // 센서 객체
    pthread_t thread;       // LedController 가 관리하는 스레드
} LedController;

/**
 * @brief LedController 생성자
 * 
 * @param control LedController 초기화 구조체
 * @param sv HttpServer (생성 되어 있어야 함)
 */
void ledControllerCreate(LedController* control, HttpServer* sv);

/**
 * @brief LED 점등 제어 POST /led/on
 * 
 * @param csock 클라이언트 소켓
 * @param req   요청 데이터
 * @param arg   LedController 객체 수신
 */
void ledOn(int csock, HttpRequest* req, void* arg);

/**
 * @brief LED 소등 제어 POST /led/off
 * 
 * @param csock 클라이언트 소켓
 * @param req   요청 데이터
 * @param arg   LedController 객체 수신
 */
void ledOff(int csock, HttpRequest* req, void* arg);

/**
 * @brief LED PWM 제어 POST /led/pwm
 * 
 * @param csock 클라이언트 소켓
 * @param req   요청 데이터
 * @param arg   LedController 객체 수신
 */
void ledPwmSet(int csock, HttpRequest* req, void* arg);

/**
 * @brief LED 모드 제어 POST /led/mode
 * 
 * @param csock 클라이언트 소켓
 * @param req   요청 데이터
 * @param arg   LedController 객체 수신
 */
void ledMode(int csock, HttpRequest* req, void* arg);

/**
 * @brief LED 전기능 제어 POST /led
 * 
 * @param csock 클라이언트 소켓
 * @param req   요청 데이터
 * @param arg   LedController 객체 수신
 */
void ledSet(int csock, HttpRequest* req, void* arg);

/**
 * @brief LED 상태 조회 GET /led
 * 
 * @param csock 클라이언트 소켓
 * @param req   요청 데이터
 * @param arg   LedController 객체 수신
 */
void ledGet(int csock, HttpRequest* req, void* arg);

/**
 * @brief CDS(조도센서) 값 조회 GET cds
 * 
 * @param csock 클라이언트 소켓
 * @param req   요청 데이터
 * @param arg   LedController 객체 수신
 */
void cdsGet(int csock, HttpRequest* req, void* arg);

#endif // __LEDCONTROLLER_H__