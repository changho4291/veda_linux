#include "yl40.h"

int createYl40(YL40* yl40, const char* device, int devId) {
    if((yl40->fd = wiringPiI2CSetup(devId))<0) {
        perror("wiringPiI2CSetup failed:\n");
        return 0;
    }

    if((yl40->fd = wiringPiI2CSetupInterface(device,devId))<0) {
        perror("wiringPiI2CSetupInterface failed:\n");
        return 0;
    }

    return 1;
}

int getCds(YL40* yl40) {
    // int prev, a2dVal;

    wiringPiI2CWrite(yl40->fd, 0x00 | CDS);
    // prev= wiringPiI2CRead(yl40->fd); 
    // a2dVal = wiringPiI2CRead(yl40->fd);

    // return a2dVal;
    return wiringPiI2CRead(yl40->fd);
}

int getTmp(YL40* yl40) {
    int prev, a2dVal;

    wiringPiI2CWrite(yl40->fd, 0x00 | TMP);
    prev= wiringPiI2CRead(yl40->fd); 
    a2dVal = wiringPiI2CRead(yl40->fd);

    return a2dVal;
}

int getPot(YL40* yl40) {
    int prev, a2dVal;

    wiringPiI2CWrite(yl40->fd, 0x00 | POT);
    prev= wiringPiI2CRead(yl40->fd); 
    a2dVal = wiringPiI2CRead(yl40->fd);

    return a2dVal;
}
