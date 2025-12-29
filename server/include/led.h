#ifndef __LED_H__
#define __LED_H__

#include <pthread.h>
#include <wiringPi.h>
#include <softPwm.h>

#define MAX_PWM 255

/**
 * @brief LED 객체
 * 
 */
typedef struct {
    int pin;        // LED GPIO 핀
    int status;     // LED ON/OFF 상태 0: OFF  1: ON
    int pwm;        // LED PWM 0 - 100
    int mode;       // CDS 연동 여부 0: 비연동, 1: 연동
    pthread_mutex_t mutex;  // LED 뮤텍스
} Led;

/**
 * @brief LED 객체 생성
 * 
 * @param led   LED 객체
 * @param pin   GPIO 핀 번호
 */
void ledCreate(Led* led, int pin);

/**
 * @brief Led 객체 소멸
 * 
 * @param led 
 */
void ledDestroy(Led* led);

/**
 * @brief led on/off 제어
 * 
 * @param led 
 * @param value 0: 소등 1: 점등
 */
void ledOnOff(Led* led, int value);

/**
 * @brief led pwm 제어
 * 
 * @param led 
 * @param value 0 ~ 100 까지 PWM 제어
 */
void ledPwm(Led* led, int value);

/**
 * @brief LED Pin 값 조회
 * 
 * @param led 
 * @return int LED GPIO
 */
int ledGetPin(Led* led);

/**
 * @brief LED ON/OFF 상태 조회
 * 
 * @param led 
 * @return int 0: 소등 1: 점등
 */
int ledGetStatus(Led* led);

/**
 * @brief LED PWM 값 조회
 * 
 * @param led 
 * @return int 0 ~ 100 까지의 pwm 데이터
 */
int ledGetPwm(Led* led);

/**
 * @brief LED 모드 조회
 * 
 * @param led 
 * @return int 0 : 수동모드     1: CDS 연동 모드
 */
int ledGetMode(Led* led);

/**
 * @brief LED 모드 설정
 * 
 * @param led 
 * @param mode 0 : 수동모드     1: CDS 연동 모드
 */
void ledSetMode(Led* led, int mode);


#endif // __LED_H__