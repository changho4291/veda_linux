
#include "controller.h"

void ledOn(int csock, HttpRequest* req) {
    char respons[BUFSIZ];

    sprintf(respons, "HTTP/1.1 200 OK\r\n"
        "Content-Type: application/json\r\n"
        "Connection: close\r\n"
        "Content-Length: 42\r\n\r\n"
        "{"
            "\"result\":1,"
            "\"status\":%d,"
            "\"pwm\":100,"
            "\"cds\":0"
        "}"
    , ledOnOff(led, 1));

    send(csock, respons, strlen(respons), 0);
}

void ledOff(int csock, HttpRequest* req) {
    char respons[BUFSIZ];

    sprintf(respons, "HTTP/1.1 200 OK\r\n"
        "Content-Type: application/json\r\n"
        "Connection: close\r\n"
        "Content-Length: 42\r\n\r\n"
        "{"
            "\"result\":1,"
            "\"status\":%d,"
            "\"pwm\":100,"
            "\"cds\":0"
        "}"
    , ledOnOff(led, 0));

    send(csock, respons, strlen(respons), 0);
}

void ledPwmSet(int csock, HttpRequest* req) {
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

void ledCds(int csock, HttpRequest* req) {
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

void ledSet(int csock, HttpRequest* req) {
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

void ledGet(int csock, HttpRequest* req) {
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

void cdsGet(int csock, HttpRequest* req) {
    char respons[BUFSIZ];

    sprintf(respons, "HTTP/1.1 200 OK\r\n"
        "Content-Type: application/json\r\n"
        "Connection: close\r\n"
        "Content-Length: 11\r\n\r\n"
        "{\"cds\":123}"
    );

    send(csock, respons, strlen(respons), 0);
}

void buzzOn(int csock, HttpRequest* req) {
    char respons[BUFSIZ];

    sprintf(respons, "HTTP/1.1 200 OK\r\n"
        "Content-Type: application/json\r\n"
        "Connection: close\r\n"
        "Content-Length: 13\r\n\r\n"
        "{\"result\":1}"
    );

    send(csock, respons, strlen(respons), 0);
}

void buzzOff(int csock, HttpRequest* req) {
    char respons[BUFSIZ];

    sprintf(respons, "HTTP/1.1 200 OK\r\n"
        "Content-Type: application/json\r\n"
        "Connection: close\r\n"
        "Content-Length: 13\r\n\r\n"
        "{\"result\":1}"
    );

    send(csock, respons, strlen(respons), 0);
}

void alaramSet(int csock, HttpRequest* req) {
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

void alaramDelete(int csock, HttpRequest* req) {
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