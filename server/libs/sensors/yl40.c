#include "yl40.h"

int yl40Create(YL40* yl40, const char* device, int devId) {
    if((yl40->fd = wiringPiI2CSetup(devId))<0) {
        perror("wiringPiI2CSetup failed:\n");
        return 0;
    }

    if((yl40->fd = wiringPiI2CSetupInterface(device,devId))<0) {
        perror("wiringPiI2CSetupInterface failed:\n");
        return 0;
    }

    pthread_mutex_init(&yl40->mutex, NULL);

    return 1;
}

void yl40Destroy(YL40* yl40) {
    pthread_mutex_destroy(&yl40->mutex);
}

int getCds(YL40* yl40) {
    int a2dVal;

    pthread_mutex_lock(&yl40->mutex);
    wiringPiI2CWrite(yl40->fd, 0x00 | CDS);
    a2dVal = wiringPiI2CRead(yl40->fd);
    pthread_mutex_unlock(&yl40->mutex);

    return wiringPiI2CRead(yl40->fd);
}

int getTmp(YL40* yl40) {
    int a2dVal;

    pthread_mutex_lock(&yl40->mutex);
    wiringPiI2CWrite(yl40->fd, 0x00 | TMP);
    a2dVal = wiringPiI2CRead(yl40->fd);
    pthread_mutex_unlock(&yl40->mutex);

    return a2dVal;
}

int getPot(YL40* yl40) {
    int a2dVal;

    pthread_mutex_lock(&yl40->mutex);
    wiringPiI2CWrite(yl40->fd, 0x00 | POT);
    a2dVal = wiringPiI2CRead(yl40->fd);
    pthread_mutex_unlock(&yl40->mutex);

    return a2dVal;
}
