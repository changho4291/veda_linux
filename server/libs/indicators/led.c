#include "led.h"

void* _ledPwmStart(void* arg);

void ledCreate(Led* led, int pin) {
    led->pin = pin;
    led->status = 0;
    led->pwm = 255;
    led->cds = 0;
    pinMode(pin, OUTPUT);
}

// void ledPwmStart(Led* led) {
//     pthread_mutex_init(&led->mutex, NULL);
//     pthread_create(&led->thread, NULL, _ledPwmStart, led);
// }

// void ledPwmJoin(Led* led) {
//     pthread_join(led->thread, NULL);
//     pthread_mutex_destroy(&led->mutex);
// }

void ledOnOff(Led* led, int value) {
    if (value) {
        softPwmCreate(led->pin, led->pwm, 255);
    } else {
        softPwmStop(led->pin);
    }
    digitalWrite(led->pin, value);
    led->status = digitalRead(led->pin);
}

void ledPwm(Led* led, int value) {
    if (value > 255) { value = 255; }
    else if (value < 0) { value = 0; }
    softPwmWrite(led->pin, value);
    led->pwm = value;
}

//--- private ---

// void* _ledPwmStart(void* arg) {
//     Led* led = (Led*)arg;
//     while (1) {
//         pthread_mutex_lock(&led->mutex);
//         softPwmWrite(led->pin, led->pwm);
//         pthread_mutex_unlock(&led->mutex);
//         delay(5);
//     }
// }