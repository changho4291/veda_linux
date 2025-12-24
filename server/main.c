#include "httpserver.h"
#include "peripheral.h"

int main(int argc, char const *argv[]) {
    HttpServer server;

    wiringPiSetup();
    // pinMode(LED, OUTPUT);

    serverCreate(&server);
    serverStart(&server);
    serverJoin(&server);
    return 0;
}