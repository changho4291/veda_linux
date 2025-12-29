#ifndef __DYNLOAD_H__
#define __DYNLOAD_H__

#include <stddef.h>

#include "httpserver.h"
#include "linkedList.h"
#include "buzz.h"
#include "led.h"
#include "fnd.h"
#include "yl40.h"

/* 성공: 0, 실패: -1 */
int dynload_init(void);

/* ===== http ===== */
extern int  (*dyn_serverCreate)(HttpServer* server);
extern void (*dyn_serverStart)(HttpServer* server);
extern void (*dyn_serverJoin)(HttpServer* server);
extern void (*dyn_setGetApi)(HttpServer* server, const char* path,
                             void (*http_func)(int, HttpRequest*, void*), void* arg);
extern void (*dyn_setPostApi)(HttpServer* server, const char* path,
                              void (*http_func)(int, HttpRequest*, void*), void* arg);
extern void (*dyn_setDeleteApi)(HttpServer* server, const char* path,
                                void (*http_func)(int, HttpRequest*, void*), void* arg);

/* ===== data_structure ===== */
extern int (*dyn_linkedListCreate)(LinkedList* list, int maxSize);
extern int (*dyn_inputNode)(LinkedList* list, void* data);
extern int (*dyn_searchNode)(LinkedList* list, int (*searchLogic)(void*, void*),
                             void* findData, void** result);
extern int (*dyn_deleteNode)(LinkedList* list, int* (*searchLogic)(void*, void*), void* removeData);

/* ===== indicators ===== */
extern void (*dyn_buzzCreate)(Buzz* buzz, int pin);
extern void (*dyn_buzzDestroy)(Buzz* buzz);
extern void (*dyn_buzzPlay)(Buzz* buzz);
extern int  (*dyn_buzzGetIsPlay)(Buzz* buzz);
extern void (*dyn_buzzPlayStop)(Buzz* buzz);

extern void (*dyn_ledCreate)(Led* led, int pin);
extern void (*dyn_ledDestroy)(Led* led);
extern void (*dyn_ledOnOff)(Led* led, int value);
extern void (*dyn_ledPwm)(Led* led, int value);
extern int  (*dyn_ledGetPin)(Led* led);
extern int  (*dyn_ledGetStatus)(Led* led);
extern int  (*dyn_ledGetPwm)(Led* led);
extern int  (*dyn_ledGetMode)(Led* led);
extern void (*dyn_ledSetMode)(Led* led, int mode);

extern void (*dyn_fndCreate)(Fnd* fnd, int a, int b, int c, int d);
extern void (*dyn_fndDestroy)(Fnd* fnd);
extern void (*dyn_fndControl)(Fnd* fnd, int num);
extern int  (*dyn_fndGetNum)(Fnd* fnd);
extern void (*dyn_fndSetNum)(Fnd* fnd, int num);

/* ===== sensors ===== */
extern int  (*dyn_yl40Create)(YL40* yl40, const char* device, int devId);
extern void (*dyn_yl40Destroy)(YL40* yl40);
extern int  (*dyn_getCds)(YL40* yl40);
extern int  (*dyn_getTmp)(YL40* yl40);
extern int  (*dyn_getPot)(YL40* yl40);

/* ===== cjson ===== */
typedef struct cJSON cJSON;

extern cJSON* (*dyn_cJSON_Parse)(const char* value);
extern cJSON* (*dyn_cJSON_GetObjectItem)(const cJSON* const object, const char* const string);
extern void   (*dyn_cJSON_Delete)(cJSON* item);
extern cJSON* (*dyn_cJSON_CreateObject)(void);
extern cJSON* (*dyn_cJSON_AddNumberToObject)(cJSON* const object, const char* const name, const double number);
extern char*  (*dyn_cJSON_PrintUnformatted)(const cJSON* item);

/*
 * 매크로로 기존 호출부를 최대한 유지.
 * - 컴파일 시 링크 타임 심볼 참조가 발생하지 않도록 매크로로 우회
 */

/* http */
#define serverCreate   (*dyn_serverCreate)
#define serverStart    (*dyn_serverStart)
#define serverJoin     (*dyn_serverJoin)
#define setGetApi      (*dyn_setGetApi)
#define setPostApi     (*dyn_setPostApi)
#define setDeleteApi   (*dyn_setDeleteApi)

/* data_structure */
#define linkedListCreate (*dyn_linkedListCreate)
#define inputNode        (*dyn_inputNode)
#define searchNode       (*dyn_searchNode)
#define deleteNode       (*dyn_deleteNode)

/* indicators */
#define buzzCreate     (*dyn_buzzCreate)
#define buzzDestroy    (*dyn_buzzDestroy)
#define buzzPlay       (*dyn_buzzPlay)
#define buzzGetIsPlay  (*dyn_buzzGetIsPlay)
#define buzzPlayStop   (*dyn_buzzPlayStop)

#define ledCreate      (*dyn_ledCreate)
#define ledDestroy     (*dyn_ledDestroy)
#define ledOnOff       (*dyn_ledOnOff)
#define ledPwm         (*dyn_ledPwm)
#define ledGetPin      (*dyn_ledGetPin)
#define ledGetStatus   (*dyn_ledGetStatus)
#define ledGetPwm      (*dyn_ledGetPwm)
#define ledGetMode     (*dyn_ledGetMode)
#define ledSetMode     (*dyn_ledSetMode)

#define fndCreate      (*dyn_fndCreate)
#define fndDestroy     (*dyn_fndDestroy)
#define fndControl     (*dyn_fndControl)
#define fndGetNum      (*dyn_fndGetNum)
#define fndSetNum      (*dyn_fndSetNum)

/* sensors */
#define yl40Create   (*dyn_yl40Create)
#define yl40Destroy  (*dyn_yl40Destroy)
#define getCds       (*dyn_getCds)
#define getTmp       (*dyn_getTmp)
#define getPot       (*dyn_getPot)

#endif /* __DYNLOAD_H__ */
