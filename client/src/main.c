#include "client.h"

#define MAXLINE 255
#define SHELL_NAME "client> "

int main(int argc, char const *argv[]) {
    char line[MAXLINE];
    char** arglist;

    if(argc < 3) {
        printf("Usage: %s <host> <port>\n", argv[0]);
        return -1;
    }

    const char* host = argv[1];
    int port = atoi(argv[2]);

    signal(SIGQUIT, SIG_IGN);
    signal(SIGTSTP, SIG_IGN);

    fputs(SHELL_NAME, stdout);

    while (fgets(line, MAXLINE, stdin)) {
        arglist = command_parse(line);

        if (arglist != NULL) {
            if (!is_builtin(arglist, host, port)) {
                printf("존재하지 않는 명령어 입니다.\n"
                    "help를 입력하여 명령어 목록을 확인하실 수 있습니다.\n");
            }
            command_freelist(arglist);
        }
        fputs(SHELL_NAME, stdout);
    }
    return 0;
}
