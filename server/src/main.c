#include "peripheral.h"

#include "httpserver.h"

#include "ledController.h"
#include "buzzController.h"

int main(int argc, char const *argv[]) {
    wiringPiSetup();
    
    HttpServer server;

    Led led;
    YL40 yl40;
    Fnd fnd;
    Buzz buzz;

    LedController ledControl;
    BuzzController buzzControl;

    serverCreate(&server);
    ledCreate(&led, LED1);
    yl40Create(&yl40, I2C_NAME, I2C_1_ID);
    fndCreate(&fnd, FND_A, FND_B, FND_C, FND_D);
    buzzCreate(&buzz, BUZZ);

    ledControllerCreate(&ledControl, &server, &led, &yl40);
    buzzControllerCreate(&buzzControl, &server, &fnd, &buzz);

    serverStart(&server);
    serverJoin(&server);

    ledDestroy(&led);
    yl40Destroy(&yl40);
    fndDestroy(&fnd);

    return 0;
}