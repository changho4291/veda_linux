#include "httpserver.h"
#include "controller.h"

#define HTTP_PORT 60000

typedef struct {
	char* path;
	void (*http_func)(int, HttpRequest*);
} HTTP_API;

HTTP_API getApi[] = {
    "/led", ledGet,
    "/cds", cdsGet,
    NULL, NULL
};

HTTP_API postApi[] = {
    "/led/on", ledOn,
    "/led/off", ledOff,
    "/led/pwm", ledPwm,
    "/led/cds", ledCds,
    "/led", ledSet,
    "/buzz/on", buzzOn,
    "/buzz/off", buzzOff,
    "/alaram", alaramSet,
    NULL, NULL
};

HTTP_API deleteApi[] = {
    "/alaram", alaramDelete,
    NULL, NULL
};

//--- private funcs ---
void* _serverStart(void* sv);
void parseHttp(char* buf, int csock);
void* _clnt_connection(void *arg);
void sendError(int csock);
void cleanup(int efd, int fd);

//--- public ---

int serverCreate(HttpServer* server) {
    server->sock = socket(AF_INET, SOCK_STREAM, 0);
    if(server->sock == -1) {
        perror("socket()");
        return 0;
    }

    int optval = 1;
    setsockopt(server->sock , SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));

    memset(&server->servaddr, 0, sizeof(server->servaddr));
    server->servaddr.sin_family = AF_INET;
    server->servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    server->servaddr.sin_port = htons(HTTP_PORT);
    if(bind(server->sock, (struct sockaddr *)&server->servaddr, sizeof(server->servaddr))==-1) {
        perror("bind()");
        return 0;
    }

    if(listen(server->sock, 10) == -1) {
        perror("listen()");
        return 0;
    }

    // epoll 생성
    if((server->efd = epoll_create1(0)) == -1) {
        perror("epoll_create");
        exit(1);
    }

    // epoll에 서버 소켓 추가
    server->ev.events = EPOLLIN;
    server->ev.data.fd = server->sock;

    if(epoll_ctl(server->efd, EPOLL_CTL_ADD, server->sock, &server->ev) == -1) {
        perror("epoll_ctl");
        exit(1);
    }

    return 1;
}

void serverStart(HttpServer* server) {
    pthread_create(&server->thread, NULL, _serverStart, server);
}

void serverJoin(HttpServer* server) {
    pthread_join(server->thread, NULL);
}

//--- private ---

void* _serverStart(void* sv) {
    HttpServer* server = (HttpServer*)sv;
    pthread_t thread;

    char buf_in[BUFSIZ];

    while(1) {
        char mesg[BUFSIZ];
        int csock;
        
        // epoll wait를 이용하여 소켓 이벤트 대기
        int event_count;
        if((event_count = epoll_wait(server->efd, server->events, MAX_EVENTS, -1))==-1) {
            perror("epoll_wait");
        }

        for (int i = 0; i < event_count; i++) {
            if(server->events[i].data.fd == server->sock) {
                /* 클라이언트의 요청을 기다린다. */
                uint32_t len = sizeof(server->cliaddr);
                csock = accept(server->sock, (struct sockaddr*)&server->cliaddr, &len);
        
                /* 네트워크 주소를 문자열로 변경 */
                inet_ntop(AF_INET, &server->cliaddr.sin_addr, mesg, BUFSIZ);
                printf("Client IP : %s:%d\n", mesg, ntohs(server->cliaddr.sin_port));
        
                // 논블럭 소켓
                // nonblock(csock);

                // epoll에 클라이언트 소켓 추가
                server->ev.events = EPOLLIN | EPOLLRDHUP | EPOLLERR | EPOLLHUP;
                server->ev.data.fd = csock;
        
                if(epoll_ctl(server->efd, EPOLL_CTL_ADD, csock, &server->ev) == -1) {
                    perror("epoll_ctl");
                }
            } else {
                // epoll 에러 또는 종료
                if (server->events[i].events & (EPOLLRDHUP | EPOLLERR | EPOLLHUP)) {
                    cleanup(server->efd, server->events[i].data.fd);
                    continue;
                }

                // epoll 수신 처리
                if (server->events[i].events & EPOLLIN) {
                    while (1) {
                        ssize_t readn = read(server->events[i].data.fd, buf_in, BUFSIZ-1);
    
                        if (readn > 0) {
                            buf_in[readn] = '\0';
                            printf("%s\n", buf_in);

                            // 읽어온 패킷을 http 파싱 함수로 보낸다
                            parseHttp(buf_in, server->events[i].data.fd);
                        }
                        
                        cleanup(server->efd, server->events[i].data.fd);
                        break;
                    }
                    continue;
                }
            }
        }
    }
}

void parseHttp(char* buf, int csock) {
    char head[BUFSIZ];
    char method[10];
    char path[2048], *ret;
    HttpRequest req;

    char* headEnd = strstr(buf, "\r\n\r\n");
    if (headEnd) {
        req.bodySize = headEnd - buf;
        strncpy(head, buf, headEnd - buf);
        head[headEnd - buf] = '\0';
        strcpy(req.body, headEnd + 4);
    }

    /* ' ' 문자로 buf를 구분해서 요청 라인의 내용(메소드)를 분리한다. */
    ret = strtok(head, "/ ");
    strcpy(method, (ret != NULL)?ret:"");

    ret = strtok(NULL, " ");                /* 요청 라인에서 경로(path)를 가져온다. */
    strcpy(path, (ret != NULL)?ret:"/");

    // http 버전 확인
    ret = strtok(NULL, "\r\n"); 

    // http 옵션 파싱
    ret = strtok(NULL, "\r\n"); 
    req.optSize = 0;
    while ( ret != NULL ) {
        strcpy(req.headOption[req.optSize], ret);
        ret = strtok(NULL, "\r\n"); 
        req.optSize++;
    }

    int i = 0, succ = 0;
    if(strcmp(method, "POST") == 0) {         /* POST 메소드일 경우를 처리한다. */
        for(; postApi[i].path != NULL; i++) {
            if(!strcmp(postApi[i].path, path)) {
                postApi[i].http_func(csock, &req);
                succ = 1;
            }
        }
    } else if(strcmp(method, "GET") == 0) {	/* GET 메소드일 경우를 처리한다. */
        for(; getApi[i].path != NULL; i++) {
            if(!strcmp(getApi[i].path, path)) {
                getApi[i].http_func(csock, &req);
                succ = 1;
            }
        }
    } else if(strcmp(method, "DELETE") == 0) {	/* DELETE 메소드일 경우를 처리한다. */
        for(; deleteApi[i].path != NULL; i++) {
            if(!strcmp(deleteApi[i].path, path)) {
                deleteApi[i].http_func(csock, &req);
                succ = 1;
            }
        }
    }

    // i가 0이면 호출하는게 없다는 의미이므로 오류
    if (!succ) { sendError(csock); }   /* 에러 메시지를 클라이언트로 보낸다. */
}

void cleanup(int efd, int fd) {
    epoll_ctl(efd, EPOLL_CTL_DEL, fd, NULL);  // epoll 제거
    close(fd);                                // fd 종료
}

void sendError(int csock) {
    /* 클라이언트로 보낼 실패에 대한 HTTP 응답 메시지 */
    char protocol[ ] = "HTTP/1.1 404 Not Found\r\n";
    char server[ ] = "Server: Netscape-Enterprise/6.0\r\n";
    // char cnt_len[ ] = "Content-Length:1024\r\n";
    char cnt_type[ ] = "Content-Type:text/html\r\n\r\n";

    /* 화면에 표시될 HTML의 내용 */
    char content1[ ] = "<html><head><title>Content Not Found</title></head>";
    char content2[ ] = "<body><font size=+5>Content Not Found</font></body></html>";

    send(csock, protocol, strlen(protocol), 0);
    send(csock, server, strlen(server), 0);
    // send(csock, cnt_len, strlen(cnt_len), 0);
    send(csock, cnt_type, strlen(cnt_type), 0);
    send(csock, content1, strlen(content1), 0);
    send(csock, content2, strlen(content2), 0);
}