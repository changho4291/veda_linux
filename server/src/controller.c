
#include "controller.h"

const char defultHeader[] = "HTTP/1.1 200 OK\r\n"
    "Content-Type: application/json\r\n"
    "Connection: close\r\n";

void controllerCreate(Controller* control, HttpServer* sv, Led* led) {
    control->sver = sv;
    control->led = led;

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

    ledOnOff(control->led, HIGH);

    char body[500]; 
    sprintf(body, "{"
            "\"status\": %d,"
            "\"pwm\": %d,"
            "\"cds\": %d"
        "}"
        , control->led->status
        , control->led->pwm
        , control->led->cds
    );

    sprintf(respons, "%s"
        "Content-Length: %d\r\n\r\n"
        "%s"
        , defultHeader, strlen(body), body);

    send(csock, respons, strlen(respons), 0);
}

void ledOff(int csock, HttpRequest* req, void* arg) {
    Controller* control = (Controller*)arg;
    char respons[BUFSIZ];

    ledOnOff(control->led, LOW);

    char body[500]; 
    sprintf(body, "{"
            "\"status\": %d,"
            "\"pwm\": %d,"
            "\"cds\": %d"
        "}"
        , control->led->status
        , control->led->pwm
        , control->led->cds
    );

    sprintf(respons, "%s"
        "Content-Length: %d\r\n\r\n"
        "%s"
        , defultHeader, strlen(body), body);

    send(csock, respons, strlen(respons), 0);
}

void ledPwmSet(int csock, HttpRequest* req, void* arg) {
    Controller* control = (Controller*)arg;
    char respons[BUFSIZ];

    char* ret;
    ret = strtok(req->body, "{}:");
    while (ret != NULL) {
printf("%s\n", ret);
         ret = strtok(NULL, "{}:");
    }
    

    // ledPwm(control->led, req.);

    char body[500]; 
    sprintf(body, "{"
            "\"status\": %d,"
            "\"pwm\": %d,"
            "\"cds\": %d"
        "}"
        , control->led->status
        , control->led->pwm
        , control->led->cds
    );

    sprintf(respons, "%s"
        "Content-Length: %d\r\n\r\n"
        "%s"
        , defultHeader, strlen(body), body);

    send(csock, respons, strlen(respons), 0);
}

void ledCds(int csock, HttpRequest* req, void* arg) {
    char respons[BUFSIZ];

    sprintf(respons, "HTTP/1.1 200 OK\r\n"
        "Content-Type: application/json\r\n"
        "Connection: close\r\n"
        "Content-Length: 42\r\n\r\n"
        "{"
            "\"result\":1,"
            "\"status\":1,"
            "\"pwm\":100,"
            "\"cds\":0"
        "}"
    );

    send(csock, respons, strlen(respons), 0);
}

void ledSet(int csock, HttpRequest* req, void* arg) {
    char respons[BUFSIZ];

    sprintf(respons, "HTTP/1.1 200 OK\r\n"
        "Content-Type: application/json\r\n"
        "Connection: close\r\n"
        "Content-Length: 42\r\n\r\n"
        "{"
            "\"result\":1,"
            "\"status\":1,"
            "\"pwm\":100,"
            "\"cds\":0"
        "}"
    );

    send(csock, respons, strlen(respons), 0);
}

void ledGet(int csock, HttpRequest* req, void* arg) {
    char respons[BUFSIZ];

    sprintf(respons, "HTTP/1.1 200 OK\r\n"
        "Content-Type: application/json\r\n"
        "Connection: close\r\n"
        "Content-Length: 42\r\n\r\n"
        "{"
            "\"result\":1,"
            "\"status\":1,"
            "\"pwm\":100,"
            "\"cds\":0"
        "}"
    );

    send(csock, respons, strlen(respons), 0);
}

void cdsGet(int csock, HttpRequest* req, void* arg) {
    char respons[BUFSIZ];

    sprintf(respons, "HTTP/1.1 200 OK\r\n"
        "Content-Type: application/json\r\n"
        "Connection: close\r\n"
        "Content-Length: 11\r\n\r\n"
        "{\"cds\":123}"
    );

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