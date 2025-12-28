#ifndef __LEDCONTROLLER_H__
#define __LEDCONTROLLER_H__

#include <pthread.h>

#include "cJSON.h"

#include "peripheral.h"
#include "httpserver.h"
#include "led.h"
#include "yl40.h"

#define MAX_LED_NUM 10

typedef struct {
    HttpServer* sver;
    Led led;
    YL40 yl40;
    pthread_t thread;
} LedController;

void ledControllerCreate(LedController* control, HttpServer* sv);

void ledOn(int csock, HttpRequest* req, void* arg);

void ledOff(int csock, HttpRequest* req, void* arg);

void ledPwmSet(int csock, HttpRequest* req, void* arg);

void ledMode(int csock, HttpRequest* req, void* arg);

void ledSet(int csock, HttpRequest* req, void* arg);

void ledGet(int csock, HttpRequest* req, void* arg);

void cdsGet(int csock, HttpRequest* req, void* arg);

#endif // __LEDCONTROLLER_H__