#ifndef __BUZZ_H__
#define __BUZZ_H__

#include <pthread.h>
#include <wiringPi.h>
#include <softTone.h>

/**
 * @brief Buzz 객체
 * 
 */
typedef struct {
    int pin;                // buzzer pin
    int isPlay;             // 부저 사용 플래그
    int stop;               // 부저 중단 플래그
    pthread_mutex_t mutex;  // 부저 전용 뮤텍스
} Buzz;

/**
 * @brief Buzz 생성자
 * 
 * @param buzz  Buzz 초기화 구조체
 * @param pin   부저가 사용할 GPIO 핀 번호
 */
void buzzCreate(Buzz* buzz, int pin);

/**
 * @brief Buzz 소멸자
 * 
 * @param buzz Buzz 객체
 */
void buzzDestroy(Buzz* buzz);

/**
 * @brief 부저 실행
 * 
 * @param buzz Buzz 객체
 */
void buzzPlay(Buzz* buzz);

/**
 * @brief 부저 실행중 확인
 * 
 * @param buzz Buzz 객체
 */
int buzzGetIsPlay(Buzz* buzz);

/**
 * @brief 부저 실행 중단
 * 
 * @param buzz Buzz 객체
 */
void buzzPlayStop(Buzz* buzz);

#endif // __BUZZ_H__