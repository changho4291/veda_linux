#include "led.h"

void ledCreate(Led* led, int pin) {
    led->pin = pin;
    led->status = 0;
    led->pwm = 100;
    led->mode = 0;
    pinMode(pin, OUTPUT);

    pthread_mutex_init(&led->mutex, NULL);
}

void ledDestroy(Led* led) {
    softPwmWrite(led->pin, 0);
    pinMode(led->pin, INPUT);
    pthread_mutex_destroy(&led->mutex);
}

void ledOnOff(Led* led, int value) {
    pthread_mutex_lock(&led->mutex);

    if (value) {
        softPwmCreate(led->pin, led->pwm, 100);
    } else {
        softPwmStop(led->pin);
    }
    digitalWrite(led->pin, value);
    led->status = digitalRead(led->pin);

    pthread_mutex_unlock(&led->mutex);
}

void ledPwm(Led* led, int value) {
    if (value > 100) { value = 100; }
    else if (value < 0) { value = 0; }

    pthread_mutex_lock(&led->mutex);

    softPwmWrite(led->pin, value);
    led->pwm = value;

    pthread_mutex_unlock(&led->mutex);
}

int ledGetPin(Led* led) {
    int pin;
    pthread_mutex_lock(&led->mutex);
    pin = led->pin;
    pthread_mutex_unlock(&led->mutex);
    return pin;
}

int ledGetStatus(Led* led) {
    int status;
    pthread_mutex_lock(&led->mutex);
    status = led->status;
    pthread_mutex_unlock(&led->mutex);
    return status;
}

int ledGetPwm(Led* led) {
    int pwm;
    pthread_mutex_lock(&led->mutex);
    pwm = led->pwm;
    pthread_mutex_unlock(&led->mutex);
    return pwm;
}

int ledGetMode(Led* led) {
    int mode;
    pthread_mutex_lock(&led->mutex);
    mode = led->mode;
    pthread_mutex_unlock(&led->mutex);
    return mode;
}

void ledSetMode(Led* led, int mode) {
    pthread_mutex_lock(&led->mutex);
    led->mode = mode;
    pthread_mutex_unlock(&led->mutex);
}