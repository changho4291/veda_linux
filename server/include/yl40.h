#ifndef __YL40_H__
#define __YL40_H__

#include <stdio.h>
#include <pthread.h>
#include <wiringPi.h>
#include <wiringPiI2C.h>

#define CDS 0
#define TMP 1
#define POT 3

typedef struct {
    int fd;
    pthread_mutex_t mutex;
} YL40;


int yl40Create(YL40* yl40, const char* device, int devId);
void yl40Destroy(YL40* yl40);

int getCds(YL40* yl40);

int getTmp(YL40* yl40);

int getPot(YL40* yl40);

#endif // __YL40_H__