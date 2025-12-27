#ifndef __LED_H__
#define __LED_H__

// #include <pthread.h>
#include <wiringPi.h>
#include <softPwm.h>

typedef struct {
    // pthread_t thread;
    // pthread_mutex_t mutex;
    int pin;
    int status;
    int pwm;
    int cds;
} Led;

#define MAX_PWM 255

void ledCreate(Led* led, int pin);

void ledOnOff(Led* led, int value);

void ledPwm(Led* led, int value);


#endif // __LED_H__