#include "client.h"

#include "cJSON.h"

int connect_tcp(const char* host, int port);

BT_TYPE btn_cmds[] = {
    "help", help_function,
    "led-on", sendLedOn,
    "led-off", sendLedOff,
    "led-pwm", sendLedPwm,
    "led-mode", sendLedMode,
    "cds", sendGetCds,
    "alarm-set", sendAlarmSet,
    "alarm-cancle", sendAlarmCancle,
    "buzz-on", sendBuzzOn,
    "buzz-off", sendBuzzOff,
    NULL, NULL
};

int is_builtin(char** arglist, const char* host, int port) {
    for(int i = 0; btn_cmds[i].cmd != NULL; i++) {
        if(!strcmp(btn_cmds[i].cmd, arglist[0])) {
            btn_cmds[i].bt_func(arglist, host, port);
            return 1;   // builtin Command;
        }
    }
    return 0;
}

void help_function(char** arglist, const char* host, int port) {
    printf("명령어 목록은 다음과 같습니다.\n\n"
    "led-on: led를 점등합니다.\n\n"
    "led-off: led를 소등 합니다.\n\n"
    "led-pwm <0 ~ 100>: led 밝기를 조절합니다.\n\n"
    "led-mode <0 - 1>: led 모드를 설정합니다.\n"
    "\t0: 수동 pwm 모드\n"
    "\t1: cds 연동 pwm 모드\n\n" 
    "cds: 현재 조도센서 값을 읽습니다.\n\n"
    "alarm-set <0 - 9>: 입력한 숫자만큼 FND가 세팅되며 카운트 이후엔 알람이 울립니다.\n\n"
    "alarm-cancle: 예약했던 알람을 취소합니다.(부저음도 같이 종료됩니다.)\n\n"
    "buzz-on: 부저를 즉시 작동합니다.\n\n"
    "buzz-off: 부저 작동을 중지합니다.\n\n"
    );
}

void sendLedOn(char** arglist, const char* host, int port) {
    int sock = connect_tcp(host, port);
    int numbytes;

    char request[1024];
    char respons[1024];

    sprintf(request, "POST /led/on HTTP/1.1\r\n"
    "User-Agent: simple-c-client/1.0\r\n"
    "Connection: close\r\n"
    "Host: %s:%d\r\n\r\n", host, port);

    send(sock, request, sizeof(request), 0);

    if((numbytes= recv(sock, respons, 1023, 0)) == -1) {
        perror("recv");
    }

    respons[numbytes] = '\0';
    printf("Received : %s\n", respons);
    close(sock);
}

void sendLedOff(char** arglist, const char* host, int port) {
    int sock = connect_tcp(host, port);
    int numbytes;

    char request[1024];
    char respons[1024];

    sprintf(request, "POST /led/off HTTP/1.1\r\n"
    "User-Agent: simple-c-client/1.0\r\n"
    "Connection: close\r\n"
    "Host: %s:%d\r\n\r\n", host, port);

    send(sock, request, sizeof(request), 0);

    if((numbytes= recv(sock, respons, 1023, 0)) == -1) {
        perror("recv");
    }

    respons[numbytes] = '\0';
    printf("Received : %s\n", respons);
    close(sock);
}

void sendLedPwm(char** arglist, const char* host, int port) {
    int sock = connect_tcp(host, port);
    int numbytes;

    if (arglist[1] == NULL) {
        printf("0 부터 100 까지의 인자를 입력해 주세요\n"
        "예: led-pwm 100\n");
        return;
    }

    char request[1024];
    char respons[1024];

    cJSON* root = cJSON_CreateObject();
    cJSON_AddNumberToObject(root, "pwm", atoi(arglist[1]));
    char* body = cJSON_PrintUnformatted(root);
    cJSON_Delete(root); // json 객체 삭제

    sprintf(request, "POST /led/pwm HTTP/1.1\r\n"
    "User-Agent: simple-c-client/1.0\r\n"
    "Connection: close\r\n"
    "Host: %s:%d\r\n"
    "Content-Length: %d\r\n\r\n"
    "%s", host, port, strlen(body), body);

    send(sock, request, sizeof(request), 0);

    if((numbytes= recv(sock, respons, 1023, 0)) == -1) {
        perror("recv");
    }

    respons[numbytes] = '\0';
    printf("Received : %s\n", respons);
    close(sock);
}

void sendLedMode(char** arglist, const char* host, int port) {
    int sock = connect_tcp(host, port);
    int numbytes;

    if (arglist[1] == NULL) {
        printf("0 또는 1을 입력해주세요\n"
        "예: led-mode 1\n");
        return;
    }

    char request[1024];
    char respons[1024];

    cJSON* root = cJSON_CreateObject();
    cJSON_AddNumberToObject(root, "mode", atoi(arglist[1]));
    char* body = cJSON_PrintUnformatted(root);
    cJSON_Delete(root); // json 객체 삭제

    sprintf(request, "POST /led/mode HTTP/1.1\r\n"
    "User-Agent: simple-c-client/1.0\r\n"
    "Connection: close\r\n"
    "Host: %s:%d\r\n"
    "Content-Length: %d\r\n\r\n"
    "%s", host, port, strlen(body), body);

    send(sock, request, sizeof(request), 0);

    if((numbytes= recv(sock, respons, 1023, 0)) == -1) {
        perror("recv");
    }

    respons[numbytes] = '\0';
    printf("Received : %s\n", respons);
    close(sock);
}

void sendGetCds(char** arglist, const char* host, int port) {
    int sock = connect_tcp(host, port);
    int numbytes;

    char request[1024];
    char respons[1024];

    sprintf(request, "GET /cds HTTP/1.1\r\n"
    "User-Agent: simple-c-client/1.0\r\n"
    "Connection: close\r\n"
    "Host: %s:%d\r\n\r\n", host, port);

    send(sock, request, sizeof(request), 0);

    if((numbytes= recv(sock, respons, 1023, 0)) == -1) {
        perror("recv");
    }

    respons[numbytes] = '\0';
    printf("Received : %s\n", respons);
    close(sock);
}

void sendAlarmSet(char** arglist, const char* host, int port) {
    int sock = connect_tcp(host, port);
    int numbytes;

    if (arglist[1] == NULL) {
        printf("0 부터 9 까지의 인자를 입력해 주세요\n"
        "예: alaram-set 9\n");
        return;
    }

    char request[1024];
    char respons[1024];

    cJSON* root = cJSON_CreateObject();
    cJSON_AddNumberToObject(root, "fnd", atoi(arglist[1]));
    char* body = cJSON_PrintUnformatted(root);
    cJSON_Delete(root); // json 객체 삭제

    sprintf(request, "POST /fnd HTTP/1.1\r\n"
    "User-Agent: simple-c-client/1.0\r\n"
    "Connection: close\r\n"
    "Host: %s:%d\r\n"
    "Content-Length: %d\r\n\r\n"
    "%s", host, port, strlen(body), body);

    send(sock, request, sizeof(request), 0);

    if((numbytes= recv(sock, respons, 1023, 0)) == -1) {
        perror("recv");
    }

    respons[numbytes] = '\0';
    printf("Received : %s\n", respons);
    close(sock);
}

void sendAlarmCancle(char** arglist, const char* host, int port) {
    int sock = connect_tcp(host, port);
    int numbytes;

    char request[1024];
    char respons[1024];

    sprintf(request, "DELETE /cds HTTP/1.1\r\n"
    "User-Agent: simple-c-client/1.0\r\n"
    "Connection: close\r\n"
    "Host: %s:%d\r\n\r\n", host, port);

    send(sock, request, sizeof(request), 0);

    if((numbytes= recv(sock, respons, 1023, 0)) == -1) {
        perror("recv");
    }

    respons[numbytes] = '\0';
    printf("Received : %s\n", respons);
    close(sock);
}

void sendBuzzOn(char** arglist, const char* host, int port) {
    int sock = connect_tcp(host, port);
    int numbytes;

    char request[1024];
    char respons[1024];

    sprintf(request, "POST /buzz HTTP/1.1\r\n"
    "User-Agent: simple-c-client/1.0\r\n"
    "Connection: close\r\n"
    "Host: %s:%d\r\n\r\n", host, port);

    send(sock, request, sizeof(request), 0);

    if((numbytes= recv(sock, respons, 1023, 0)) == -1) {
        perror("recv");
    }

    respons[numbytes] = '\0';
    printf("Received : %s\n", respons);
    close(sock);
}

void sendBuzzOff(char** arglist, const char* host, int port) {
    int sock = connect_tcp(host, port);
    int numbytes;

    char request[1024];
    char respons[1024];

    sprintf(request, "DELETE /buzz HTTP/1.1\r\n"
    "User-Agent: simple-c-client/1.0\r\n"
    "Connection: close\r\n"
    "Host: %s:%d\r\n\r\n", host, port);

    send(sock, request, sizeof(request), 0);

    if((numbytes= recv(sock, respons, 1023, 0)) == -1) {
        perror("recv");
    }

    respons[numbytes] = '\0';
    printf("Received : %s\n", respons);
    close(sock);
}

/** private **/

int connect_tcp(const char *host, int port) {
    int sockfd;
    socklen_t addr_len;
    struct hostent *he;
    struct sockaddr_in server_addr;

    if((he = gethostbyname(host)) == NULL) {
        perror("gethostbyname");
        return -1;
    }

    if((sockfd= socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket");
        return -1;
    }

    server_addr.sin_family= AF_INET;
    server_addr.sin_port= htons(port);
    server_addr.sin_addr= *((struct in_addr*)he->h_addr_list[0]);
    printf("[ %s ]\n",(char*) inet_ntoa(server_addr.sin_addr));
    memset(&(server_addr.sin_zero), '\0',8);

    if(connect(sockfd, (struct sockaddr*)&server_addr, sizeof(struct sockaddr))== -1) {
        perror("connect");
        return -1;
    }

    return sockfd; // 실패 시 -1
}