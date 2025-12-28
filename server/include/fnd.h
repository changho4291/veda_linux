#ifndef __FND_H__
#define __FND_H__

#include <pthread.h>
#include <wiringPi.h>

typedef struct {
    int pins[4];
    int num;
    pthread_mutex_t mutex;
} Fnd;


void fndCreate(Fnd* fnd, int a, int b, int c, int d);

void fndDestroy(Fnd* fnd);

void fndControl(Fnd* fnd, int num);

int fndGetNum(Fnd* fnd);
void fndSetNum(Fnd* fnd, int num);

#endif // __FND_H__