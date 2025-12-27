#include "httpserver.h"
#include "led.h"
#include "controller.h"

int main(int argc, char const *argv[]) {
    wiringPiSetup();
    
    HttpServer server;
    Led led;
    Controller controll;

    serverCreate(&server);
    ledCreate(&led, LED1);
    controllerCreate(&controll, &server, &led);

    serverStart(&server);

    serverJoin(&server);
    return 0;
}