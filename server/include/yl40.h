#ifndef __YL40_H__
#define __YL40_H__

#include <stdio.h>
#include <pthread.h>
#include <wiringPi.h>
#include <wiringPiI2C.h>

#define CDS 0
#define TMP 1
#define POT 3

/**
 * @brief YL40 센서 객체
 * 
 */
typedef struct {
    int fd;     // i2c fd
    pthread_mutex_t mutex;  // YL40 전용 뮤텍스
} YL40;

/**
 * @brief YL40 생성자
 * 
 * @param yl40      yl40 초기화 구조체
 * @param device    i2c 장치 이름
 * @param devId     i2c 장치 id
 * @return int      0 실패 1 성공
 */
int yl40Create(YL40* yl40, const char* device, int devId);

/**
 * @brief YL40 소멸자
 * 
 * @param yl40 
 */
void yl40Destroy(YL40* yl40);

/**
 * @brief CDS 값 조회
 * 
 * @param yl40 
 * @return int CDS 값
 */
int getCds(YL40* yl40);

/**
 * @brief 온도 값 조회
 * 
 * @param yl40 
 * @return int 온도 값
 */
int getTmp(YL40* yl40);

/**
 * @brief 가변 저항
 * 
 * @param yl40 
 * @return int 가변 저항
 */
int getPot(YL40* yl40);

#endif // __YL40_H__