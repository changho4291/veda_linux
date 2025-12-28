#ifndef __YL40_H__
#define __YL40_H__

#include <stdio.h>
#include <wiringPi.h>
#include <wiringPiI2C.h>

#define CDS 0
#define TMP 1
#define POT 3

typedef struct {
    int fd;
} YL40;


int createYl40(YL40* yl40, const char* device, int devId);

int getCds(YL40* yl40);

int getTmp(YL40* yl40);

int getPot(YL40* yl40);

#endif // __YL40_H__