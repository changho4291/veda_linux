#include "peripheral.h"

#include "httpserver.h"
#include "led.h"
#include "yl40.h"
#include "controller.h"

int main(int argc, char const *argv[]) {
    wiringPiSetup();
    
    HttpServer server;
    Led led;
    YL40 yl40;
    Controller controll;

    serverCreate(&server);
    ledCreate(&led, LED1);
    yl40Create(&yl40, I2C_NAME, I2C_1_ID);
    controllerCreate(&controll, &server, &led, &yl40);

    serverStart(&server);

    serverJoin(&server);

    ledDestroy(&led);
    yl40Destroy(&yl40);

    return 0;
}