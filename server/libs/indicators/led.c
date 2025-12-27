#include "led.h"

void ledCreate(Led* led, int pin) {
    led->pin = pin;
    led->status = 0;
    led->pwm = 100;
    led->cds = 0;
    pinMode(pin, OUTPUT);
}

void ledOnOff(Led* led, int value) {
    if (value) {
        softPwmCreate(led->pin, led->pwm, 100);
    } else {
        softPwmStop(led->pin);
    }
    digitalWrite(led->pin, value);
    led->status = digitalRead(led->pin);
}

void ledPwm(Led* led, int value) {
    if (value > 100) { value = 100; }
    else if (value < 0) { value = 0; }
    softPwmWrite(led->pin, value);
    led->pwm = value;
}