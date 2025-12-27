#include "yl40.h"

// void* cdsThreadFnc(void* yl40);
// void* tmlThreadFnc(void* yl40);
// void* potThreadFnc(void* yl40);

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

// void yl40threadStart(YL40* yl40) {
//     pthread_create(&yl40->cdsThread, NULL, cdsThreadFnc, yl40);
//     pthread_create(&yl40->tmpThread, NULL, tmlThreadFnc, yl40);
//     pthread_create(&yl40->potThread, NULL, potThreadFnc, yl40);
// }

// void yl40threadJoin(YL40* yl40) {
//     pthread_join(yl40->cdsThread, NULL);
//     pthread_join(yl40->tmpThread, NULL);
//     pthread_join(yl40->potThread, NULL);
// }

int getCds(YL40* yl40) {
    int prev, a2dVal;

    wiringPiI2CWrite(yl40->fd, 0x00 | CDS);
    prev= wiringPiI2CRead(yl40->fd); 
    a2dVal = wiringPiI2CRead(yl40->fd);

    return a2dVal;
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

/** private **/

// void* cdsThreadFnc(void* arg) {
//     YL40* yl40 = arg;
//     int prev, a2dVal;
//     while(1) {
//         wiringPiI2CWrite(yl40->fd, 0x00 | a2dChannel);       // 0000_0000 
//         prev= wiringPiI2CRead(fd);     // Previously byte, garvage
//         a2dVal = wiringPiI2CRead(fd);
//         printf("[%d] prev= %d, ", cnt, prev); 
//         printf("a2dVal = %d, ", a2dVal);
        
//         if(a2dVal < threshold) { 
//             printf("Bright!!\n");
//         } else {
//             printf("Dark!!\n"); 
//         }
//         delay(1000);
//         cnt++; 
//     }
// }

// void* tmlThreadFnc(void* arg) {

// }

// void* potThreadFnc(void* arg) {

// }
