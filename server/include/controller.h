#ifndef __CONTROLLER_H__
#define __CONTROLLER_H__

#include <pthread.h>

#include "cJSON.h"

#include "httpserver.h"
#include "led.h"
#include "yl40.h"

#define MAX_LED_NUM 10

typedef struct {
    HttpServer* sver;
    Led* led;
    YL40* yl40;
    pthread_t thread;
    pthread_mutex_t mutex;
    int isThreadRun;
} Controller;

void controllerCreate(Controller* control, HttpServer* sv, Led* led, YL40* yl40);

void ledOn(int csock, HttpRequest* req, void* arg);

void ledOff(int csock, HttpRequest* req, void* arg);

void ledPwmSet(int csock, HttpRequest* req, void* arg);

void ledMode(int csock, HttpRequest* req, void* arg);

void ledSet(int csock, HttpRequest* req, void* arg);

void ledGet(int csock, HttpRequest* req, void* arg);

void cdsGet(int csock, HttpRequest* req, void* arg);

void buzzOn(int csock, HttpRequest* req, void* arg);

void buzzOff(int csock, HttpRequest* req, void* arg);

void alaramSet(int csock, HttpRequest* req, void* arg);

void alaramDelete(int csock, HttpRequest* req, void* arg);
#endif // __CONTROLLER_H__