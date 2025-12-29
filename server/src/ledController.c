
#include "ledController.h"

/* dlopen 기반 런타임 바인딩(함수 호출은 매크로로 우회) */
#include "dynload_cjson.h"

/** private functions **/

/**
 * @brief LED CDS MODE를 실행하기 위한 스레드
 * 
 * @param arg LedController*
 * @return void* 
 */
void* _ledCdsModeThread(void* arg);

/**
 * @brief LED 작동 모드 설정
 * 
 * @param control 
 * @param mode      0: PWM 수동 조작 1: PWM CDS 연동
 */
void ledModeSet(LedController* control, int mode);

/** public **/

void ledControllerCreate(LedController* control, HttpServer* sv) {
    control->sver = sv;

    // 인디케이터 및 센서값 초기화
    ledCreate(&control->led, LED1);
    yl40Create(&control->yl40, I2C_NAME, I2C_1_ID);

    // HTTP RESTful API 설정
    setGetApi(sv, "/led", ledGet, (void*)control);
    setGetApi(sv, "/cds", cdsGet, (void*)control);

    setPostApi(sv, "/led/on", ledOn, (void*)control);
    setPostApi(sv, "/led/off", ledOff, (void*)control);
    setPostApi(sv, "/led/pwm", ledPwmSet, (void*)control);
    setPostApi(sv, "/led/mode", ledMode, (void*)control);
    setPostApi(sv, "/led", ledSet, (void*)control);
}

void ledOn(int csock, HttpRequest* req, void* arg) {
    LedController* control = (LedController*)arg;
    char respons[BUFSIZ];

    // led On 수행
    ledOnOff(&control->led, HIGH);

    // JSON 포멧으로 결과 작성(body)
    cJSON* root = cJSON_CreateObject();

    cJSON_AddNumberToObject(root, "status", control->led.status);
    cJSON_AddNumberToObject(root, "pwm", control->led.pwm);
    cJSON_AddNumberToObject(root, "mode", control->led.mode);

    char* body = cJSON_PrintUnformatted(root);

    cJSON_Delete(root); // json 객체 삭제

    // HTTP 포멧에 맞게 작성
    sprintf(respons, "%s"
        "Content-Length: %d\r\n\r\n"
        "%s"
        , HTTP_DEF_HEAD, strlen(body), body);

    // 결과 송신
    send(csock, respons, strlen(respons), 0);
}

void ledOff(int csock, HttpRequest* req, void* arg) {
    LedController* control = (LedController*)arg;
    char respons[BUFSIZ];

    // led Off 수행
    ledOnOff(&control->led, LOW);

    // JSON 포멧으로 결과 작성(body)
    cJSON* root = cJSON_CreateObject();

    cJSON_AddNumberToObject(root, "status", ledGetStatus(&control->led));
    cJSON_AddNumberToObject(root, "pwm", ledGetPwm(&control->led));
    cJSON_AddNumberToObject(root, "mode", ledGetMode(&control->led));

    char* body = cJSON_PrintUnformatted(root);

    cJSON_Delete(root); // json 객체 삭제

    // HTTP 포멧에 맞게 작성
    sprintf(respons, "%s"
        "Content-Length: %d\r\n\r\n"
        "%s"
        , HTTP_DEF_HEAD, strlen(body), body);

    // 결과 송신
    send(csock, respons, strlen(respons), 0);
}

void ledPwmSet(int csock, HttpRequest* req, void* arg) {
    LedController* control = (LedController*)arg;
    char respons[BUFSIZ];

    // 수신값을 JSON으로 파싱
    cJSON* root = cJSON_Parse(req->body);
    cJSON* pwm = cJSON_GetObjectItem(root, "pwm");
    
    ledPwm(&control->led, pwm->valueint);
    
    cJSON_Delete(root);
    
    // JSON 포멧으로 결과 작성(body)
    root = cJSON_CreateObject();

    cJSON_AddNumberToObject(root, "status", ledGetStatus(&control->led));
    cJSON_AddNumberToObject(root, "pwm", ledGetPwm(&control->led));
    cJSON_AddNumberToObject(root, "mode", ledGetMode(&control->led));

    char* body = cJSON_PrintUnformatted(root);

    cJSON_Delete(root); // json 객체 삭제

    // HTTP 포멧에 맞게 작성
    sprintf(respons, "%s"
        "Content-Length: %d\r\n\r\n"
        "%s"
        , HTTP_DEF_HEAD, strlen(body), body);

    // 결과 송신
    send(csock, respons, strlen(respons), 0);
}

void ledMode(int csock, HttpRequest* req, void* arg) {
    LedController* control = (LedController*) arg;
    char respons[BUFSIZ];

    // 수신값을 JSON으로 파싱
    cJSON* root = cJSON_Parse(req->body);
    cJSON* mode = cJSON_GetObjectItem(root, "mode");
    
    ledModeSet(control, mode->valueint);

    cJSON_Delete(root);

    // JSON 포멧으로 결과 작성(body)
    root = cJSON_CreateObject();

    cJSON_AddNumberToObject(root, "status", ledGetStatus(&control->led));
    cJSON_AddNumberToObject(root, "pwm", ledGetPwm(&control->led));
    cJSON_AddNumberToObject(root, "mode", ledGetMode(&control->led));

    char* body = cJSON_PrintUnformatted(root);

    cJSON_Delete(root); // json 객체 삭제

    // HTTP 포멧에 맞게 작성
    sprintf(respons, "%s"
        "Content-Length: %d\r\n\r\n"
        "%s"
        , HTTP_DEF_HEAD, strlen(body), body);

    // 결과 송신
    send(csock, respons, strlen(respons), 0);
}

void ledSet(int csock, HttpRequest* req, void* arg) {
    LedController* control = (LedController*) arg;
    char respons[BUFSIZ];

    // 수신값을 JSON으로 파싱
    cJSON* root = cJSON_Parse(req->body);
    cJSON* status = cJSON_GetObjectItem(root, "status");
    cJSON* pwm = cJSON_GetObjectItem(root, "pwm");
    cJSON* mode = cJSON_GetObjectItem(root, "mode");
    
    ledOnOff(&control->led, status->valueint);
    ledPwm(&control->led, pwm->valueint);
    ledModeSet(control, mode->valueint);

    cJSON_Delete(root);
    
    // JSON 포멧으로 결과 작성(body)
    root = cJSON_CreateObject();

    cJSON_AddNumberToObject(root, "status", ledGetStatus(&control->led));
    cJSON_AddNumberToObject(root, "pwm", ledGetPwm(&control->led));
    cJSON_AddNumberToObject(root, "mode", ledGetMode(&control->led));

    char* body = cJSON_PrintUnformatted(root);

    cJSON_Delete(root); // json 객체 삭제

    // HTTP 포멧에 맞게 작성
    sprintf(respons, "%s"
        "Content-Length: %d\r\n\r\n"
        "%s"
        , HTTP_DEF_HEAD, strlen(body), body);

    // 결과 송신
    send(csock, respons, strlen(respons), 0);
}

void ledGet(int csock, HttpRequest* req, void* arg) {
    LedController* control = (LedController*)arg;
    char respons[BUFSIZ];

    // JSON 포멧으로 결과 작성(body)
    cJSON* root = cJSON_CreateObject();

    cJSON_AddNumberToObject(root, "status", ledGetStatus(&control->led));
    cJSON_AddNumberToObject(root, "pwm", ledGetPwm(&control->led));
    cJSON_AddNumberToObject(root, "mode", ledGetMode(&control->led));

    char* body = cJSON_PrintUnformatted(root);

    cJSON_Delete(root); // json 객체 삭제

    // HTTP 포멧에 맞게 작성
    sprintf(respons, "%s"
        "Content-Length: %d\r\n\r\n"
        "%s"
        , HTTP_DEF_HEAD, strlen(body), body);

    // 결과 송신
    send(csock, respons, strlen(respons), 0);
}

void cdsGet(int csock, HttpRequest* req, void* arg) {
    LedController* control = (LedController*)arg;
    char respons[BUFSIZ];

    // YL40으로부터
    int cds = getCds(&control->yl40);

    // 결과 값 생성
    cJSON* root = cJSON_CreateObject();
    cJSON_AddNumberToObject(root, "cds", cds);
    char* body = cJSON_PrintUnformatted(root);
    cJSON_Delete(root); // json 객체 삭제

    // HTTP 포멧에 맞게 작성
    sprintf(respons, "%s"
        "Content-Length: %d\r\n\r\n"
        "%s"
        , HTTP_DEF_HEAD, strlen(body), body);

    // 결과 송신
    send(csock, respons, strlen(respons), 0);
}

/** private **/

void ledModeSet(LedController* control, int mode) {
    switch (mode) {
    case 0: // 일반 모드
        // 실행중인 thread가 있다면 종료
        ledSetMode(&control->led, 0);
        break;

    case 1: // CDS 연동 모드
        if (ledGetMode(&control->led)) { break; }

        ledSetMode(&control->led, 1);

        pthread_create(&control->thread, NULL, _ledCdsModeThread, control);
        pthread_detach(control->thread);
        break;
    
    default:
        break;
    }
}

void* _ledCdsModeThread(void* arg) {
    LedController* control = (LedController*) arg;

    while (ledGetMode(&control->led)) {
        // CDS 값 0 - 255 를 PWM의 0 - 100 으로 맞춤
        int cdsValue = getCds(&control->yl40);
        ledPwm(&control->led, (cdsValue * 100) / 255);

        delay(500);
    }
}