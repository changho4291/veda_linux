#ifndef __LED_H__
#define __LED_H__

#include <wiringPi.h>
#include <softPwm.h>

#define MAX_PWM 255

typedef struct {
    int pin;
    int status;
    int pwm;
    int mode;
} Led;

void ledCreate(Led* led, int pin);

void ledOnOff(Led* led, int value);

void ledPwm(Led* led, int value);


#endif // __LED_H__