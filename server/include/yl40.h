#ifndef __YL40_H__
#define __YL40_H__

#include <stdio.h>
#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <pthread.h>

#define CDS 0
#define TMP 1
#define POT 3

typedef struct {
    int fd;
    // pthread_t cdsThread;
    // pthread_t tmpThread;
    // pthread_t potThread;
    // pthread_mutex_t cdsMutex;
    // pthread_mutex_t tmpMutex;
    // pthread_mutex_t potMutex;
} YL40;


int createYl40(YL40* yl40, const char* device, int devId);

// void yl40threadStart(YL40* yl40);

// void yl40threadJoin(YL40* yl40);

int getCds(YL40* yl40);

int getTmp(YL40* yl40);

int getPot(YL40* yl40);

#endif // __YL40_H__