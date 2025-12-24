#ifndef __HTTPSERVER_H__
#define __HTTPSERVER_H__

#include "httpobject.h"

int serverCreate(HttpServer* server);
void serverStart(HttpServer* server);
void serverJoin(HttpServer* server);

#endif // __HTTPSERVER_H__