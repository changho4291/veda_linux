#ifndef __LED_H__
#define __LED_H__

#include <pthread.h>
#include <wiringPi.h>
#include <softPwm.h>

#define MAX_PWM 255

typedef struct {
    int pin;
    int status;
    int pwm;
    int mode;
    pthread_mutex_t mutex;
} Led;

void ledCreate(Led* led, int pin);

void ledDestroy(Led* led);

void ledOnOff(Led* led, int value);

void ledPwm(Led* led, int value);

int ledGetPin(Led* led);

int ledGetStatus(Led* led);

int ledGetPwm(Led* led);

int ledGetMode(Led* led);
void ledSetMode(Led* led, int mode);


#endif // __LED_H__