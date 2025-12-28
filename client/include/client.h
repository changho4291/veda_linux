#ifndef __CLIENT_H__
#define __CLIENT_H__

#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <stdlib.h>
#include <errno.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

typedef struct bt_type {
    char *cmd;
    void (*bt_func)(char **, const char*, int);
} BT_TYPE;

/** parsing.c **/
char** command_parse(char *line);
void command_freelist(char **arglist);

/** httpClient.c **/
int is_builtin(char** arglist, const char* host, int port);
void help_function(char** arglist, const char* host, int port);
void sendLedOn(char** arglist, const char* host, int port);
void sendLedOff(char** arglist, const char* host, int port);
void sendLedPwm(char** arglist, const char* host, int port);
void sendLedMode(char** arglist, const char* host, int port);
void sendGetCds(char** arglist, const char* host, int port);
void sendAlarmSet(char** arglist, const char* host, int port);
void sendAlarmCancle(char** arglist, const char* host, int port);
void sendBuzzOn(char** arglist, const char* host, int port);
void sendBuzzOff(char** arglist, const char* host, int port);

#endif // __CLIENT_H__