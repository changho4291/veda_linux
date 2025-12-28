#ifndef __BUZZCONTROLLER_H__
#define __BUZZCONTROLLER_H__

#include <stdio.h>
#include <pthread.h>

#include "cJSON.h"

#include "peripheral.h"
#include "httpserver.h"
#include "fnd.h"
#include "buzz.h"

typedef struct {
    HttpServer* sver;
    Fnd fnd;
    Buzz buzz;
    pthread_t thread;
} BuzzController;

void buzzControllerCreate(BuzzController* control, HttpServer* sv);

void fndSet(int csock, HttpRequest* req, void* arg);

void fndDelete(int csock, HttpRequest* req, void* arg);

void buzzOn(int csock, HttpRequest* req, void* arg);

void buzzOff(int csock, HttpRequest* req, void* arg);

#endif // __BUZZCONTROLLER_H__