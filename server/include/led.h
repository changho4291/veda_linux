#ifndef __LED_H__
#define __LED_H__

#include <pthread.h>
#include <softPwm.h>

#include "peripheral.h"
#include "controller.h"

typedef struct {
    pthread_t thread;
    pthread_mutex_t mutex;
    int pin;
    int status;
    int pwm;
    int cds;
} Led;

#define MAX_PWM 255

void createLed(Led* led, int pin);
void ledPwmStart(Led* led);
void ledPwmJoin(Led* led);

int ledOnOff(Led* led, int value);

int ledPwm(Led* led, int value);


#endif // __LED_H__