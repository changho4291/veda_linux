#include "fnd.h"

int fndNumber[10][4] = {
    {0, 0, 0, 0},   // 0
    {0, 0, 0, 1},   // 1
    {0, 0, 1, 0},   // 2
    {0, 0, 1, 1},   // 3
    {0, 1, 0, 0},   // 4
    {0, 1, 0, 1},   // 5
    {0, 1, 1, 0},   // 6
    {0, 1, 1, 1},   // 7
    {1, 0, 0, 0},   // 8
    {1, 0, 0, 1}    // 9
};

void fndCreate(Fnd* fnd, int a, int b, int c, int d) {
    fnd->pins[0] = a;
    fnd->pins[1] = b;
    fnd->pins[2] = c;
    fnd->pins[3] = d;
    fnd->num = 0;

    // FND 초기화
    for (int i = 0; i < 4; i++) {
        pinMode(fnd->pins[i], OUTPUT);
        digitalWrite(fnd->pins[i], fndNumber[0][i] ? HIGH : LOW);
    }

    pthread_mutex_init(&fnd->mutex, NULL);
}

void fndDestroy(Fnd* fnd) {
    pthread_mutex_destroy(&fnd->mutex);
}

void fndControl(Fnd* fnd, int num) {
    pthread_mutex_lock(&fnd->mutex);
    if (num < 0) {
        fnd->num = 0;
    } else if (num > 9) {
        fnd->num = num;
    } else {
        fnd->num = num;
    }
    pthread_mutex_unlock(&fnd->mutex);

    for (int i = 0; i < 4; i++) {
        digitalWrite(fnd->pins[i], fndNumber[num][i] ? HIGH : LOW);
    }

    pthread_mutex_lock(&fnd->mutex);
    if (fnd->num > 0) { fnd->num--; }
    pthread_mutex_unlock(&fnd->mutex);
}

int fndGetNum(Fnd* fnd) {
    int num;
    pthread_mutex_lock(&fnd->mutex);
    num = fnd->num;
    pthread_mutex_unlock(&fnd->mutex);
    return num;
}

void fndSetNum(Fnd* fnd, int num) {
    pthread_mutex_lock(&fnd->mutex);
    fnd->num = num;
    pthread_mutex_unlock(&fnd->mutex);
}