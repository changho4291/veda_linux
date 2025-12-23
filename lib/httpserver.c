#include "httpserver.h"

#define HTTP_PORT 60000

// 메인페이지 path: "/"
void mainPage(int, void*);

HTTP_API getApi[] = {
    "/", mainPage,
    NULL, NULL
};

HTTP_API postApi[] = {
    NULL, NULL
};

//--- private funcs ---
void* _serverStart(void* sv);
void parseHttp(char* buf, int csock);
void* _clnt_connection(void *arg);
void sendError(FILE* fp);

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
            if(server->events[i].data.fd = server->sock) {
                /* 클라이언트의 요청을 기다린다. */
                uint32_t len = sizeof(server->cliaddr);
                csock = accept(server->sock, (struct sockaddr*)&server->cliaddr, &len);
        
                /* 네트워크 주소를 문자열로 변경 */
                inet_ntop(AF_INET, &server->cliaddr.sin_addr, mesg, BUFSIZ);
                printf("Client IP : %s:%d\n", mesg, ntohs(server->cliaddr.sin_port));
        
                // epoll에 클라이언트 소켓 추가
                server->ev.events = EPOLLIN | EPOLLRDHUP | EPOLLERR | EPOLLHUP;
                server->ev.data.fd = csock;
        
                if(epoll_ctl(server->efd, EPOLL_CTL_ADD, csock, &server->ev) == -1) {
                    perror("epoll_ctl");
                }
            } else {
                // epoll 수신 처리
                if (server->events[i].events & EPOLLIN) {
                    int readn = read(server->events[i].data.fd, buf_in, BUFSIZ);

                    if (readn <= 0) {
                        cleanup(server->events[i].data.fd);
                        continue;
                    }

                    buf_in[readn] = '\0';
                    printf("%s\n", buf_in);
                    // TODO 읽어온 패킷을 http 파싱 함수로 보낸다
                    parseHttp(buf_in, server->events[i].data.fd);
                    continue;
                } 

                // TODO 잠만 HTTP는 한번 송수신 받고나면 바로 종료하는데?
                
                // epoll 에러 또는 종료
                if (server->events[i].events & (EPOLLRDHUP | EPOLLERR | EPOLLHUP)) {
                    cleanup(server->events[i].data.fd);
                    continue;
                }
            }
        }

    }
}

void parseHttp(char* buf, int csock) {
    char method[BUFSIZ], type[BUFSIZ];
    char path[BUFSIZ], *ret;

    /* ' ' 문자로 buf를 구분해서 요청 라인의 내용(메소드)를 분리한다. */
    ret = strtok(buf, "/ ");
    strcpy(method, (ret != NULL)?ret:"");

    ret = strtok(NULL, " ");                /* 요청 라인에서 경로(path)를 가져온다. */
    strcpy(path, (ret != NULL)?ret:"/");

    int i = 0;
    if(strcmp(method, "POST") == 0) {         /* POST 메소드일 경우를 처리한다. */
        for(; postApi[i].path != NULL; i++) {
            if(!strcmp(postApi[i].path, path)) {
                postApi[i].http_func(csock, NULL);
            }
        }
    } else if(strcmp(method, "GET") == 0) {	/* GET 메소드일 경우를 처리한다. */
        for(; getApi[i].path != NULL; i++) {
            if(!strcmp(getApi[i].path, path)) {
                getApi[i].http_func(csock, NULL);
            }
        }
    }
}

void* _clnt_connection(void *arg) {
    int csock = *((int*)arg);
    FILE *clnt_read, *clnt_write;
    char reg_line[BUFSIZ], reg_buf[BUFSIZ];
    char method[BUFSIZ], type[BUFSIZ];
    char path[BUFSIZ], *ret;

    /* 파일 디스크립터를 FILE 스트림으로 변환한다. */
    clnt_read = fdopen(csock, "r");
    clnt_write = fdopen(dup(csock), "w");

    /* 한 줄의 문자열을 읽어서 reg_line 변수에 저장한다. */
    fgets(reg_line, BUFSIZ, clnt_read);
    
    /* reg_line 변수에 문자열을 화면에 출력한다. */
    fputs(reg_line, stdout);

    /* ' ' 문자로 reg_line을 구분해서 요청 라인의 내용(메소드)를 분리한다. */
    ret = strtok(reg_line, "/ ");
    strcpy(method, (ret != NULL)?ret:"");

    ret = strtok(NULL, " ");                /* 요청 라인에서 경로(path)를 가져온다. */
    strcpy(path, (ret != NULL)?ret:"/");

    int i = 0;
    if(strcmp(method, "POST") == 0) {         /* POST 메소드일 경우를 처리한다. */
        for(; postApi[i].path != NULL; i++) {
            if(!strcmp(postApi[i].path, path)) {
                postApi[i].http_func(clnt_write);
            }
        }
    } else if(strcmp(method, "GET") == 0) {	/* GET 메소드일 경우를 처리한다. */
        for(; getApi[i].path != NULL; i++) {
            if(!strcmp(getApi[i].path, path)) {
                getApi[i].http_func(clnt_write);
            }
        }
    }

    // TODO i가 0이면 호출하는게 없다는 의미이므로 오류
    // if (!i) { sendError(clnt_write); }   /* 에러 메시지를 클라이언트로 보낸다. */

    fclose(clnt_read);              /* 파일의 스트림을 닫는다. */
    fclose(clnt_write);
    pthread_exit(0);                /* 스레드를 종료시킨다. */

    return (void*)NULL;
}

void mainPage(int csock, void* fp) {
    FILE* clnt_write = fp;  // TODO 이건 삭제

    /* 클라이언트로 보낼 성공에 대한 응답 메시지 */
    char protocol[ ] = "HTTP/1.1 200 OK\r\n";
    char server[ ] = "Server:Netscape-Enterprise/6.0\r\n";
    char cnt_type[ ] = "Content-Type:text/html\r\n";
    char end[ ] = "\r\n";
    char body[BUFSIZ];
    sprintf(body, 
    "<html>"
            "<head>"
                "<title>Hello World</title>"
            "</head>"
            "<body>"
                "<h1>Sensor</h1>"
                "<p>CTS: <span id=\"cds\">%.3f</span></p>"
                "<p>Temperature: <span id=\"temp\">%.3f</span></p>"
                "<p>Potentiometer: <span id=\"vr\">%.3f</span></p>"

                "<h2>LED Control</h2>"
                "<form action=\"index.html\" method=\"GET\" \"onSubmit=\"document.reload()\"><table>"
                    "<button type=\"submit\" name=\"state\">LED ON</button>"
                    "<button type=\"submit\" name=\"state\">LED OFF</button>"
                "</form>"
            "</body>"
            "</html>"
    , 0.1, 0.1, 0.1);

    int fd, len;

    fputs(protocol, fp);
    fputs(server, fp);
    fputs(cnt_type, fp);
    fputs(end, fp);
    fputs(body, fp);
}

void sendError(FILE* fp) {
    /* 클라이언트로 보낼 실패에 대한 HTTP 응답 메시지 */
    char protocol[ ] = "HTTP/1.1 400 Bad Request\r\n";
    char server[ ] = "Server: Netscape-Enterprise/6.0\r\n";
    char cnt_len[ ] = "Content-Length:1024\r\n";
    char cnt_type[ ] = "Content-Type:text/html\r\n\r\n";

    /* 화면에 표시될 HTML의 내용 */
    char content1[ ] = "<html><head><title>BAD Connection</title></head>";
    char content2[ ] = "<body><font size=+5>Bad Request</font></body></html>";
    printf("send_error\n");

    fputs(protocol, fp);
    fputs(server, fp);
    fputs(cnt_len, fp);
    fputs(cnt_type, fp);
    fputs(content1, fp);
    fputs(content2, fp);
    fflush(fp);
}