#ifndef __FND_H__
#define __FND_H__

#include <pthread.h>
#include <wiringPi.h>

/**
 * @brief Fnd 객체
 * 
 */
typedef struct {
    int pins[4];    // FND 7447 
    int num;        // FND 카운트 NUM
    pthread_mutex_t mutex;  // FND 뮤택스
} Fnd;

/**
 * @brief FND 생성자
 * 
 * @param fnd   FND 초기화 구조체
 * @param a     7447 A의 GPIO 핀번호
 * @param b     7447 B의 GPIO 핀번호
 * @param c     7447 C의 GPIO 핀번호
 * @param d     7447 D의 GPIO 핀번호
 */
void fndCreate(Fnd* fnd, int a, int b, int c, int d);

/**
 * @brief FND 소멸자
 * 
 * @param fnd   FND 객체
 */
void fndDestroy(Fnd* fnd);

/**
 * @brief FND 제어
 * 
 * @param fnd   FND 객체
 * @param num   숫자 (0 ~ 9)
 */
void fndControl(Fnd* fnd, int num);

/**
 * @brief FND 설정된 숫자 조회
 * 
 * @param fnd   FND 객체
 * @return int  FND 에 설정된 숫자
 */
int fndGetNum(Fnd* fnd);

/**
 * @brief FND 숫자 설정
 * 
 * @param fnd   FND 객체
 * @param num   FND에 설정할 숫자
 */
void fndSetNum(Fnd* fnd, int num);

#endif // __FND_H__