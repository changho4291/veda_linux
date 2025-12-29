#include "buzz.h"

#define TOTAL 32    // 학교종의 전체 계이름 수

const int notes[] = {           // 학교종을 연주하기 위한 계이름
        391, 391, 440, 440, 391, 391, 329.63, 329.63, \
        391, 391, 329.63, 329.63, 293.66, 293.66, 293.66, 0, \
        391, 391, 440, 440, 391, 391, 329.63, 329.63, \
        391, 329.63, 293.66, 329.63, 261.63, 261.63, 261.63, 0
};

void buzzCreate(Buzz* buzz, int pin) {
    buzz->pin = pin;
    buzz->isPlay = 0;
    buzz->stop = 0;
    
    softToneCreate(pin);    // 톤 출력을 위한 GPIO 설정

    pthread_mutex_init(&buzz->mutex, NULL);
}

void buzzDestroy(Buzz* buzz) {
    pthread_mutex_destroy(&buzz->mutex);
}

void buzzPlay(Buzz* buzz) {
    pthread_mutex_lock(&buzz->mutex);
    buzz->isPlay = 1;
    pthread_mutex_unlock(&buzz->mutex);

    for (int i = 0; i < TOTAL; i++) {
        pthread_mutex_lock(&buzz->mutex);
        if (buzz->stop) {
            buzz->stop = 0;
            pthread_mutex_unlock(&buzz->mutex);
            break;
        }
        pthread_mutex_unlock(&buzz->mutex);
        softToneWrite(buzz->pin, notes[i]);  // 톤 출력 : 학교종 연주
        delay(280); // 음의 전체 길이만큼 출력되도록 대기
    }
    softToneWrite(buzz->pin, 0);

    pthread_mutex_lock(&buzz->mutex);
    buzz->isPlay = 0;
    pthread_mutex_unlock(&buzz->mutex);
}

int buzzGetIsPlay(Buzz* buzz) {
    int isPlay;
    pthread_mutex_lock(&buzz->mutex);
    isPlay = buzz->isPlay;
    pthread_mutex_unlock(&buzz->mutex);
    return isPlay;
}

void buzzPlayStop(Buzz* buzz) {
    pthread_mutex_lock(&buzz->mutex);
    buzz->stop = 1;
    pthread_mutex_unlock(&buzz->mutex);
}