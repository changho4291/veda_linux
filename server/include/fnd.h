#ifndef __FND_H__
#define __FND_H__

#include <pthread.h>
#include <wiringPi.h>

/**
 * @brief Fnd 객체
 * 
 */
typedef struct {
    int pins[4];    // FND 7447 
    int num;        // FND 카운트 NUM
    pthread_mutex_t mutex;  // FND 뮤택스
} Fnd;

void fndCreate(Fnd* fnd, int a, int b, int c, int d);

void fndDestroy(Fnd* fnd);

void fndControl(Fnd* fnd, int num);

int fndGetNum(Fnd* fnd);
void fndSetNum(Fnd* fnd, int num);

#endif // __FND_H__