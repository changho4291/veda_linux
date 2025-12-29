#include "dynload.h"

#include <stdio.h>
#include <dlfcn.h>
#include <stdlib.h>

/*
 * 라이브러리 로딩 순서
 *  - data_structure: http가 내부적으로 linkedList 심볼을 필요로 함
 *  - indicators/sensors: 외부 장치 제어(wiringPi) 사용
 *  - cjson: 컨트롤러에서 직접 사용
 *  - http: 마지막
 */

static void* g_h_data_structure = NULL;
static void* g_h_indicators = NULL;
static void* g_h_sensors = NULL;
static void* g_h_http = NULL;
static void* g_h_cjson = NULL;

/* ===== http ===== */
int  (*dyn_serverCreate)(HttpServer* server) = NULL;
void (*dyn_serverStart)(HttpServer* server) = NULL;
void (*dyn_serverJoin)(HttpServer* server) = NULL;
void (*dyn_setGetApi)(HttpServer* server, const char* path,
                      void (*http_func)(int, HttpRequest*, void*), void* arg) = NULL;
void (*dyn_setPostApi)(HttpServer* server, const char* path,
                       void (*http_func)(int, HttpRequest*, void*), void* arg) = NULL;
void (*dyn_setDeleteApi)(HttpServer* server, const char* path,
                         void (*http_func)(int, HttpRequest*, void*), void* arg) = NULL;

/* ===== data_structure ===== */
int (*dyn_linkedListCreate)(LinkedList* list, int maxSize) = NULL;
int (*dyn_inputNode)(LinkedList* list, void* data) = NULL;
int (*dyn_searchNode)(LinkedList* list, int (*searchLogic)(void*, void*),
                      void* findData, void** result) = NULL;
int (*dyn_deleteNode)(LinkedList* list, int* (*searchLogic)(void*, void*), void* removeData) = NULL;

/* ===== indicators ===== */
void (*dyn_buzzCreate)(Buzz* buzz, int pin) = NULL;
void (*dyn_buzzDestroy)(Buzz* buzz) = NULL;
void (*dyn_buzzPlay)(Buzz* buzz) = NULL;
int  (*dyn_buzzGetIsPlay)(Buzz* buzz) = NULL;
void (*dyn_buzzPlayStop)(Buzz* buzz) = NULL;

void (*dyn_ledCreate)(Led* led, int pin) = NULL;
void (*dyn_ledDestroy)(Led* led) = NULL;
void (*dyn_ledOnOff)(Led* led, int value) = NULL;
void (*dyn_ledPwm)(Led* led, int value) = NULL;
int  (*dyn_ledGetPin)(Led* led) = NULL;
int  (*dyn_ledGetStatus)(Led* led) = NULL;
int  (*dyn_ledGetPwm)(Led* led) = NULL;
int  (*dyn_ledGetMode)(Led* led) = NULL;
void (*dyn_ledSetMode)(Led* led, int mode) = NULL;

void (*dyn_fndCreate)(Fnd* fnd, int a, int b, int c, int d) = NULL;
void (*dyn_fndDestroy)(Fnd* fnd) = NULL;
void (*dyn_fndControl)(Fnd* fnd, int num) = NULL;
int  (*dyn_fndGetNum)(Fnd* fnd) = NULL;
void (*dyn_fndSetNum)(Fnd* fnd, int num) = NULL;

/* ===== sensors ===== */
int  (*dyn_yl40Create)(YL40* yl40, const char* device, int devId) = NULL;
void (*dyn_yl40Destroy)(YL40* yl40) = NULL;
int  (*dyn_getCds)(YL40* yl40) = NULL;
int  (*dyn_getTmp)(YL40* yl40) = NULL;
int  (*dyn_getPot)(YL40* yl40) = NULL;

/* ===== cjson ===== */
cJSON* (*dyn_cJSON_Parse)(const char* value) = NULL;
cJSON* (*dyn_cJSON_GetObjectItem)(const cJSON* const object, const char* const string) = NULL;
void   (*dyn_cJSON_Delete)(cJSON* item) = NULL;
cJSON* (*dyn_cJSON_CreateObject)(void) = NULL;
cJSON* (*dyn_cJSON_AddNumberToObject)(cJSON* const object, const char* const name, const double number) = NULL;
char*  (*dyn_cJSON_PrintUnformatted)(const cJSON* item) = NULL;

static int _bind_sym(void* h, const char* sym, void** out) {
    dlerror();
    void* p = dlsym(h, sym);
    const char* e = dlerror();
    if (e) {
        fprintf(stderr, "dlsym failed: %s: %s\n", sym, e);
        return -1;
    }
    *out = p;
    return 0;
}

int dynload_init(void) {
    g_h_data_structure = dlopen("/home/veda/veda_test/libs/libdata_structure.so", RTLD_LAZY);
    if (!g_h_data_structure) {
        fprintf(stderr, "dlopen failed: libdata_structure.so: %s\n", dlerror());
        return -1;
    }
    g_h_indicators = dlopen("/home/veda/veda_test/libs/libindicators.so", RTLD_LAZY);
    if (!g_h_indicators) {
        fprintf(stderr, "dlopen failed: libindicators.so: %s\n", dlerror());
        return -1;
    }
    g_h_sensors = dlopen("/home/veda/veda_test/libs/libsensors.so", RTLD_LAZY);
    if (!g_h_sensors) {
        fprintf(stderr, "dlopen failed: libsensors.so: %s\n", dlerror());
        return -1;
    }
    g_h_cjson = dlopen("/home/veda/veda_test/libs/libcjson.so", RTLD_LAZY);
    if (!g_h_cjson) {
        /* 일부 배포환경에서는 SONAME(libcjson.so.1)만 존재할 수 있음 */
        g_h_cjson = dlopen("/home/veda/veda_test/libs/libcjson.so.1", RTLD_LAZY);
    }
    if (!g_h_cjson) {
        fprintf(stderr, "dlopen failed: libcjson.so: %s\n", dlerror());
        return -1;
    }
    g_h_http = dlopen("/home/veda/veda_test/libs/libhttp.so", RTLD_LAZY);
    if (!g_h_http) {
        fprintf(stderr, "dlopen failed: libhttp.so: %s\n", dlerror());
        return -1;
    }

    /* ---- bind symbols ---- */
    if (_bind_sym(g_h_http, "serverCreate", (void**)&dyn_serverCreate) < 0) return -1;
    if (_bind_sym(g_h_http, "serverStart", (void**)&dyn_serverStart) < 0) return -1;
    if (_bind_sym(g_h_http, "serverJoin", (void**)&dyn_serverJoin) < 0) return -1;
    if (_bind_sym(g_h_http, "setGetApi", (void**)&dyn_setGetApi) < 0) return -1;
    if (_bind_sym(g_h_http, "setPostApi", (void**)&dyn_setPostApi) < 0) return -1;
    if (_bind_sym(g_h_http, "setDeleteApi", (void**)&dyn_setDeleteApi) < 0) return -1;

    if (_bind_sym(g_h_data_structure, "linkedListCreate", (void**)&dyn_linkedListCreate) < 0) return -1;
    if (_bind_sym(g_h_data_structure, "inputNode", (void**)&dyn_inputNode) < 0) return -1;
    if (_bind_sym(g_h_data_structure, "searchNode", (void**)&dyn_searchNode) < 0) return -1;
    if (_bind_sym(g_h_data_structure, "deleteNode", (void**)&dyn_deleteNode) < 0) return -1;

    if (_bind_sym(g_h_indicators, "buzzCreate", (void**)&dyn_buzzCreate) < 0) return -1;
    if (_bind_sym(g_h_indicators, "buzzDestroy", (void**)&dyn_buzzDestroy) < 0) return -1;
    if (_bind_sym(g_h_indicators, "buzzPlay", (void**)&dyn_buzzPlay) < 0) return -1;
    if (_bind_sym(g_h_indicators, "buzzGetIsPlay", (void**)&dyn_buzzGetIsPlay) < 0) return -1;
    if (_bind_sym(g_h_indicators, "buzzPlayStop", (void**)&dyn_buzzPlayStop) < 0) return -1;

    if (_bind_sym(g_h_indicators, "ledCreate", (void**)&dyn_ledCreate) < 0) return -1;
    if (_bind_sym(g_h_indicators, "ledDestroy", (void**)&dyn_ledDestroy) < 0) return -1;
    if (_bind_sym(g_h_indicators, "ledOnOff", (void**)&dyn_ledOnOff) < 0) return -1;
    if (_bind_sym(g_h_indicators, "ledPwm", (void**)&dyn_ledPwm) < 0) return -1;
    if (_bind_sym(g_h_indicators, "ledGetPin", (void**)&dyn_ledGetPin) < 0) return -1;
    if (_bind_sym(g_h_indicators, "ledGetStatus", (void**)&dyn_ledGetStatus) < 0) return -1;
    if (_bind_sym(g_h_indicators, "ledGetPwm", (void**)&dyn_ledGetPwm) < 0) return -1;
    if (_bind_sym(g_h_indicators, "ledGetMode", (void**)&dyn_ledGetMode) < 0) return -1;
    if (_bind_sym(g_h_indicators, "ledSetMode", (void**)&dyn_ledSetMode) < 0) return -1;

    if (_bind_sym(g_h_indicators, "fndCreate", (void**)&dyn_fndCreate) < 0) return -1;
    if (_bind_sym(g_h_indicators, "fndDestroy", (void**)&dyn_fndDestroy) < 0) return -1;
    if (_bind_sym(g_h_indicators, "fndControl", (void**)&dyn_fndControl) < 0) return -1;
    if (_bind_sym(g_h_indicators, "fndGetNum", (void**)&dyn_fndGetNum) < 0) return -1;
    if (_bind_sym(g_h_indicators, "fndSetNum", (void**)&dyn_fndSetNum) < 0) return -1;

    if (_bind_sym(g_h_sensors, "yl40Create", (void**)&dyn_yl40Create) < 0) return -1;
    if (_bind_sym(g_h_sensors, "yl40Destroy", (void**)&dyn_yl40Destroy) < 0) return -1;
    if (_bind_sym(g_h_sensors, "getCds", (void**)&dyn_getCds) < 0) return -1;
    if (_bind_sym(g_h_sensors, "getTmp", (void**)&dyn_getTmp) < 0) return -1;
    if (_bind_sym(g_h_sensors, "getPot", (void**)&dyn_getPot) < 0) return -1;

    if (_bind_sym(g_h_cjson, "cJSON_Parse", (void**)&dyn_cJSON_Parse) < 0) return -1;
    if (_bind_sym(g_h_cjson, "cJSON_GetObjectItem", (void**)&dyn_cJSON_GetObjectItem) < 0) return -1;
    if (_bind_sym(g_h_cjson, "cJSON_Delete", (void**)&dyn_cJSON_Delete) < 0) return -1;
    if (_bind_sym(g_h_cjson, "cJSON_CreateObject", (void**)&dyn_cJSON_CreateObject) < 0) return -1;
    if (_bind_sym(g_h_cjson, "cJSON_AddNumberToObject", (void**)&dyn_cJSON_AddNumberToObject) < 0) return -1;
    if (_bind_sym(g_h_cjson, "cJSON_PrintUnformatted", (void**)&dyn_cJSON_PrintUnformatted) < 0) return -1;
}