
#include "controller.h"

const char HTTP_DEF_HEAD[] = "HTTP/1.1 200 OK\r\n"
    "Content-Type: application/json\r\n"
    "Connection: close\r\n";

void controllerCreate(Controller* control, HttpServer* sv, Led* led, YL40* yl40) {
    control->sver = sv;
    control->led = led;
    control->yl40 = yl40;

    setGetApi(sv, "/led", ledGet, (void*)control);
    setGetApi(sv, "/cds", cdsGet, (void*)control);

    setPostApi(sv, "/led/on", ledOn, (void*)control);
    setPostApi(sv, "/led/off", ledOff, (void*)control);
    setPostApi(sv, "/led/pwm", ledPwmSet, (void*)control);
    setPostApi(sv, "/led/cds", ledCds, (void*)control);
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
    cJSON_AddNumberToObject(root, "cds", control->led->cds);

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

    cJSON_AddNumberToObject(root, "status", control->led->status);
    cJSON_AddNumberToObject(root, "pwm", control->led->pwm);
    cJSON_AddNumberToObject(root, "cds", control->led->cds);

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

    cJSON_AddNumberToObject(root, "status", control->led->status);
    cJSON_AddNumberToObject(root, "pwm", control->led->pwm);
    cJSON_AddNumberToObject(root, "cds", control->led->cds);

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

void ledCds(int csock, HttpRequest* req, void* arg) {
    char respons[BUFSIZ];

    // TODO 작업 남음

    sprintf(respons, "HTTP/1.1 200 OK\r\n"
        "Content-Type: application/json\r\n"
        "Connection: close\r\n"
        "Content-Length: 42\r\n\r\n"
        "{"
            "\"status\":1,"
            "\"pwm\":100,"
            "\"cds\":0"
        "}"
    );

    send(csock, respons, strlen(respons), 0);
}

void ledSet(int csock, HttpRequest* req, void* arg) {
    char respons[BUFSIZ];

    // TODO 작업 남음

    sprintf(respons, "HTTP/1.1 200 OK\r\n"
        "Content-Type: application/json\r\n"
        "Connection: close\r\n"
        "Content-Length: 42\r\n\r\n"
        "{"
            "\"status\":1,"
            "\"pwm\":100,"
            "\"cds\":0"
        "}"
    );

    send(csock, respons, strlen(respons), 0);
}

void ledGet(int csock, HttpRequest* req, void* arg) {
    Controller* control = (Controller*)arg;
    char respons[BUFSIZ];

    // JSON 포멧으로 결과 작성(body)
    cJSON* root = cJSON_CreateObject();

    cJSON_AddNumberToObject(root, "status", control->led->status);
    cJSON_AddNumberToObject(root, "pwm", control->led->pwm);
    cJSON_AddNumberToObject(root, "cds", control->led->cds);

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