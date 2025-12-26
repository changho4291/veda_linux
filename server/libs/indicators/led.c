#include "led.h"

void* _ledPwmStart(void* arg);

void ledCreate(Led* led, int pin) {
    led->pin = pin;
    led->status = 0;
    led->pwm = 255;
    led->cds = 0;
    softPwmCreate(pin, 0, 255);
}

void ledPwmStart(Led* led) {
    pthread_mutex_init(&led->mutex, NULL);
    pthread_create(&led->thread, NULL, _ledPwmStart, led);
}

void ledPwmJoin(Led* led) {
    pthread_join(led->thread, NULL);
    pthread_mutex_destroy(&led->mutex);
}

int ledOnOff(Led* led, int value) {
    digitalWrite(led->pin, value);
    return digitalRead(led->pin);
}

//--- private ---

void* _ledPwmStart(void* arg) {
    Led* led = (Led*)arg;
    while (1) {
        pthread_mutex_lock(&led->mutex);
        softPwmWrite(led->pin, led->pwm);
        pthread_mutex_unlock(&led->mutex);
        delay(5);
    }
}