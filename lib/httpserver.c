#include "httpserver.h"

#define HTTP_PORT 60000

// 메인페이지 path: "/"
void mainPage(void*);

HTTP_API getApi[] = {
    "/", mainPage,
    NULL, NULL
};

HTTP_API postApi[] = {
    NULL, NULL
};

void* _serverStart(void* sv);
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

    while(1) {
        char mesg[BUFSIZ];
        int csock;

        /* 클라이언트의 요청을 기다린다. */
        uint32_t len = sizeof(server->cliaddr);
        csock = accept(server->sock, (struct sockaddr*)&server->cliaddr, &len);

        /* 네트워크 주소를 문자열로 변경 */
        inet_ntop(AF_INET, &server->cliaddr.sin_addr, mesg, BUFSIZ);
        printf("Client IP : %s:%d\n", mesg, ntohs(server->cliaddr.sin_port));

        /* 클라이언트의 요청이 들어오면 스레드를 생성하고 클라이언트의 요청을 처리한다. */
        pthread_create(&thread, NULL, _clnt_connection, &csock);
        pthread_join(thread, NULL);
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

void mainPage(void* fp) {
    FILE* clnt_write = fp;

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