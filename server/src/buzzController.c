#include "buzzController.h"

void _setFndAlarm(BuzzController* control, int num);
void* _fndAlarmThread(void* arg);

void _playMusic(BuzzController* control);
void* _musicThread(void* arg);

/** public **/

void buzzControllerCreate(BuzzController* control, HttpServer* sv
    , Fnd* fnd, Buzz* buzz) {
    control->sver = sv;
    control->fnd = fnd;
    control->buzz = buzz;

    setPostApi(sv, "/fnd", fndSet, (void*)control);
    setDeleteApi(sv, "/fnd", fndDelete, (void*)control);

    setPostApi(sv, "/buzz", buzzOn, (void*)control);
    setDeleteApi(sv, "/buzz", buzzOff, (void*)control);
}

void fndSet(int csock, HttpRequest* req, void* arg) {
    BuzzController* control = (BuzzController*) arg;
    char respons[BUFSIZ];

    // 수신값을 JSON으로 파싱
    cJSON* root = cJSON_Parse(req->body);
    cJSON* fnd = cJSON_GetObjectItem(root, "fnd");

    _setFndAlarm(control, fnd->valueint);

    cJSON_Delete(root);

    // HTTP 포멧에 맞게 작성
    sprintf(respons, "%s"
        "Content-Length: %d\r\n\r\n"
        "%s"
        , HTTP_DEF_HEAD, strlen(req->body), req->body);

    // 결과 송신
    send(csock, respons, strlen(respons), 0);
}

void fndDelete(int csock, HttpRequest* req, void* arg) {
    BuzzController* control = (BuzzController*) arg;
    char respons[BUFSIZ];

    fndSetNum(control->fnd, 0);
    buzzPlayStop(control->buzz);

    // HTTP 포멧에 맞게 작성
    sprintf(respons, "%s"
        "\r\n"
        , HTTP_DEF_HEAD);

    // 결과 송신
    send(csock, respons, strlen(respons), 0);
}

void buzzOn(int csock, HttpRequest* req, void* arg) {
    BuzzController* control = (BuzzController*) arg;
    char respons[BUFSIZ];



    // HTTP 포멧에 맞게 작성
    sprintf(respons, "%s"
        "\r\n"
        , HTTP_DEF_HEAD);

    // 결과 송신
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

/** private **/

void _setFndAlarm(BuzzController* control, int num) {
    if (fndGetNum(control->fnd) != 0) {
        fndSetNum(control->fnd, num);
        return;
    }

    fndSetNum(control->fnd, num);
    pthread_create(&control->thread, NULL, _fndAlarmThread, control);
    pthread_detach(control->thread);
}

void* _fndAlarmThread(void* arg) {
    BuzzController* control = (BuzzController*) arg;

    // FND 카운트 다운
    int num;
    while ((num = fndGetNum(control->fnd))) {
        fndControl(control->fnd, num);
        delay(1000);
    }
    fndControl(control->fnd, 0);

    // 음악 시작
    buzzPlay(control->buzz);
}

void _playMusic(BuzzController* control) {
    
}

void* _musicThread(void* arg) {
    
}