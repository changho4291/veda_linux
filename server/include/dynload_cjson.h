#ifndef __DYNLOAD_CJSON_H__
#define __DYNLOAD_CJSON_H__

/*
 * cJSON 함수 호출을 dlopen 바인딩으로 우회하기 위한 매크로 헤더.
 * 반드시 cJSON.h가 먼저 include 된 뒤에 include 해야 한다.
 */

#include "dynload.h"

/* cjson */
#define cJSON_Parse             (*dyn_cJSON_Parse)
#define cJSON_GetObjectItem     (*dyn_cJSON_GetObjectItem)
#define cJSON_Delete            (*dyn_cJSON_Delete)
#define cJSON_CreateObject      (*dyn_cJSON_CreateObject)
#define cJSON_AddNumberToObject (*dyn_cJSON_AddNumberToObject)
#define cJSON_PrintUnformatted  (*dyn_cJSON_PrintUnformatted)

#endif /* __DYNLOAD_CJSON_H__ */
