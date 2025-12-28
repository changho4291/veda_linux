#include "buzz.h"

#define TOTAL 32    // 학교종의 전체 계이름 수

const int notes[] = {           // 학교종을 연주하기 위한 계이름
        391, 391, 440, 440, 391, 391, 329.63, 329.63, \
        391, 391, 329.63, 329.63, 293.66, 293.66, 293.66, 0, \
        391, 391, 440, 440, 391, 391, 329.63, 329.63, \
        391, 329.63, 293.66, 329.63, 261.63, 261.63, 261.63, 0
};

void buzzCreate(Buzz* fnd, int pin) {
    fnd->pin = pin;
    fnd->isPlay = 0;
    fnd->stop = 0;
    
    softToneCreate(pin);    // 톤 출력을 위한 GPIO 설정

    pthread_mutex_init(&fnd->mutex, NULL);
}

void buzzDestroy(Buzz* fnd) {
    pthread_mutex_destroy(&fnd->mutex);
}

void buzzPlay(Buzz* fnd) {
    pthread_mutex_lock(&fnd->mutex);
    fnd->isPlay = 1;
    pthread_mutex_unlock(&fnd->mutex);

    for (int i = 0; i < TOTAL; i++) {
        pthread_mutex_lock(&fnd->mutex);
        if (fnd->stop) {
            fnd->stop = 0;
            pthread_mutex_unlock(&fnd->mutex);
            break;
        }
        pthread_mutex_unlock(&fnd->mutex);
        softToneWrite(fnd->pin, notes[i]);  // 톤 출력 : 학교종 연주
        delay(280); // 음의 전체 길이만큼 출력되도록 대기
    }
    softToneWrite(fnd->pin, 0);

    pthread_mutex_lock(&fnd->mutex);
    fnd->isPlay = 0;
    pthread_mutex_unlock(&fnd->mutex);
}

int buzzGetIsPlay(Buzz* fnd) {
    int isPlay;
    pthread_mutex_lock(&fnd->mutex);
    isPlay = fnd->isPlay;
    pthread_mutex_unlock(&fnd->mutex);
    return isPlay;
}

void buzzPlayStop(Buzz* fnd) {
    pthread_mutex_lock(&fnd->mutex);
    fnd->stop = 1;
    pthread_mutex_unlock(&fnd->mutex);
}