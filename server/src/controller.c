
#include "controller.h"

const char HTTP_DEF_HEAD[] = "HTTP/1.1 200 OK\r\n"
    "Content-Type: application/json\r\n"
    "Connection: close\r\n";

/** private functions **/

void* _ledCdsModeThread(void* arg);

void ledModeSet(Controller* control, int mode);

/** public **/

void controllerCreate(Controller* control, HttpServer* sv, Led* led, YL40* yl40) {
    control->sver = sv;
    control->led = led;
    control->yl40 = yl40;

    setGetApi(sv, "/led", ledGet, (void*)control);
    setGetApi(sv, "/cds", cdsGet, (void*)control);

    setPostApi(sv, "/led/on", ledOn, (void*)control);
    setPostApi(sv, "/led/off", ledOff, (void*)control);
    setPostApi(sv, "/led/pwm", ledPwmSet, (void*)control);
    setPostApi(sv, "/led/mode", ledMode, (void*)control);
    setPostApi(sv, "/led", ledSet, (void*)control);
    setPostApi(sv, "/buzz/on", buzzOn, (void*)control);
    setPostApi(sv, "/buzz/off", buzzOff, (void*)control);
    setPostApi(sv, "/alaram", alaramSet, (void*)control);

    setDeleteApi(sv, "/alaram", alaramDelete, (void*)control);
}

void ledOn(int csock, HttpRequest* req, void* arg) {
    Controller* control = (Controller*)arg;
    char respons[BUFSIZ];

    // led On 수행
    ledOnOff(control->led, HIGH);

    // JSON 포멧으로 결과 작성(body)
    cJSON* root = cJSON_CreateObject();

    cJSON_AddNumberToObject(root, "status", control->led->status);
    cJSON_AddNumberToObject(root, "pwm", control->led->pwm);
    cJSON_AddNumberToObject(root, "mode", control->led->mode);

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
    Controller* control = (Controller*)arg;
    char respons[BUFSIZ];

    // led Off 수행
    ledOnOff(control->led, LOW);

    // JSON 포멧으로 결과 작성(body)
    cJSON* root = cJSON_CreateObject();

    cJSON_AddNumberToObject(root, "status", ledGetStatus(control->led));
    cJSON_AddNumberToObject(root, "pwm", ledGetPwm(control->led));
    cJSON_AddNumberToObject(root, "mode", ledGetMode(control->led));

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
    Controller* control = (Controller*)arg;
    char respons[BUFSIZ];

    // 수신값을 JSON으로 파싱
    cJSON* root = cJSON_Parse(req->body);
    cJSON* pwm = cJSON_GetObjectItem(root, "pwm");
    cJSON_Delete(root);


    ledPwm(control->led, pwm->valueint);
    
    // JSON 포멧으로 결과 작성(body)
    root = cJSON_CreateObject();

    cJSON_AddNumberToObject(root, "status", ledGetStatus(control->led));
    cJSON_AddNumberToObject(root, "pwm", ledGetPwm(control->led));
    cJSON_AddNumberToObject(root, "mode", ledGetMode(control->led));

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
    Controller* control = (Controller*) arg;
    char respons[BUFSIZ];

    // 수신값을 JSON으로 파싱
    cJSON* root = cJSON_Parse(req->body);
    cJSON* mode = cJSON_GetObjectItem(root, "mode");
    cJSON_Delete(root);

    ledModeSet(control, mode->valueint);

    // JSON 포멧으로 결과 작성(body)
    root = cJSON_CreateObject();

    cJSON_AddNumberToObject(root, "status", ledGetStatus(control->led));
    cJSON_AddNumberToObject(root, "pwm", ledGetPwm(control->led));
    cJSON_AddNumberToObject(root, "mode", ledGetMode(control->led));

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
    Controller* control = (Controller*) arg;
    char respons[BUFSIZ];

    // TODO 작업 남음

    sprintf(respons, "HTTP/1.1 200 OK\r\n"
        "Content-Type: application/json\r\n"
        "Connection: close\r\n"
        "Content-Length: 42\r\n\r\n"
        "{"
            "\"status\":1,"
            "\"pwm\":100,"
            "\"mode\":0"
        "}"
    );

    send(csock, respons, strlen(respons), 0);
}

void ledGet(int csock, HttpRequest* req, void* arg) {
    Controller* control = (Controller*)arg;
    char respons[BUFSIZ];

    // JSON 포멧으로 결과 작성(body)
    cJSON* root = cJSON_CreateObject();

    cJSON_AddNumberToObject(root, "status", ledGetStatus(control->led));
    cJSON_AddNumberToObject(root, "pwm", ledGetPwm(control->led));
    cJSON_AddNumberToObject(root, "mode", ledGetMode(control->led));

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
    Controller* control = (Controller*)arg;
    char respons[BUFSIZ];

    // YL40으로부터
    int cds = getCds(control->yl40);

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

void buzzOn(int csock, HttpRequest* req, void* arg) {
    char respons[BUFSIZ];

    sprintf(respons, "HTTP/1.1 200 OK\r\n"
        "Content-Type: application/json\r\n"
        "Connection: close\r\n"
        "Content-Length: 13\r\n\r\n"
        "{\"result\":1}"
    );

    send(csock, respons, strlen(respons), 0);
}

void buzzOff(int csock, HttpRequest* req, void* arg) {
    char respons[BUFSIZ];

    sprintf(respons, "HTTP/1.1 200 OK\r\n"
        "Content-Type: application/json\r\n"
        "Connection: close\r\n"
        "Content-Length: 13\r\n\r\n"
        "{\"result\":1}"
    );

    send(csock, respons, strlen(respons), 0);
}

void alaramSet(int csock, HttpRequest* req, void* arg) {
    char respons[BUFSIZ];

    sprintf(respons, "HTTP/1.1 200 OK\r\n"
        "Content-Type: application/json\r\n"
        "Connection: close\r\n"
        "Content-Length: 21\r\n\r\n"
        "{"
            "\"result\":1,"
            "\"sec\":9"
        "}"
    );

    send(csock, respons, strlen(respons), 0);
}

void alaramDelete(int csock, HttpRequest* req, void* arg) {
    char respons[BUFSIZ];

    sprintf(respons, "HTTP/1.1 200 OK\r\n"
        "Content-Type: application/json\r\n"
        "Connection: close\r\n"
        "Content-Length: 13\r\n\r\n"
        "{"
            "\"result\":1"
        "}"
    );

    send(csock, respons, strlen(respons), 0);
}


/** private **/

void ledModeSet(Controller* control, int mode) {
    switch (mode) {
    case 0: // 일반 모드
        // 실행중인 thread가 있다면 종료
        ledSetMode(control->led, 0);
        break;

    case 1: // CDS 연동 모드
        if (ledGetMode(control->led)) { break; }

        ledSetMode(control->led, 1);

        pthread_create(&control->thread, NULL, _ledCdsModeThread, control);
        pthread_detach(control->thread);
        break;
    
    default:
        break;
    }
}

void* _ledCdsModeThread(void* arg) {
    Controller* control = (Controller*) arg;

    while (ledGetMode(control->led)) {

        int cdsValue = getCds(control->yl40);
        ledPwm(control->led, (cdsValue * 100) / 255);

        delay(500);
    }
}