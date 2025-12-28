#ifndef __BUZZ_H__
#define __BUZZ_H__

#include <pthread.h>
#include <wiringPi.h>
#include <softTone.h>

typedef struct {
    int pin;
    int isPlay;
    pthread_mutex_t mutex;
} Buzz;

void buzzCreate(Buzz* fnd, int pin);

void buzzDestroy(Buzz* fnd);

void buzzPlay(Buzz* fnd);

int buzzGetIsPlay(Buzz* fnd);

void buzzPlayStop(Buzz* fnd);

#endif // __BUZZ_H__