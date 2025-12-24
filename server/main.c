#include "httpserver.h"

int main(int argc, char const *argv[]) {
    HttpServer server;
    serverCreate(&server);
    serverStart(&server);
    serverJoin(&server);
    return 0;
}