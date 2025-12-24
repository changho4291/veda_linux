#ifndef __CONTROLLER_H__
#define __CONTROLLER_H__

#include "httpobject.h"
#include "led.h"

#define MAX_LED_NUM 10

typedef struct {
    Led* led;
} DataPack;

void ledOn(int csock, HttpRequest* req);

void ledOff(int csock, HttpRequest* req);

void ledPwmSet(int csock, HttpRequest* req);

void ledCds(int csock, HttpRequest* req);

void ledSet(int csock, HttpRequest* req);

void ledGet(int csock, HttpRequest* req);

void cdsGet(int csock, HttpRequest* req);

void buzzOn(int csock, HttpRequest* req);

void buzzOff(int csock, HttpRequest* req);

void alaramSet(int csock, HttpRequest* req);

void alaramDelete(int csock, HttpRequest* req);
#endif // __CONTROLLER_H__