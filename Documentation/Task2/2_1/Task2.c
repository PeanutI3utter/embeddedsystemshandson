#include "SWD.h"


#define SWDIO 6
#define SWDCK 5



int main()
{
    DEBUG = 1;
    DEBUG_DETAIL = 0;
    if(gpioInitialise() < 0)
    {
        printf("[Error] Failed to initialise pigpio\n");
        exit(0);
    }

    gpioSetMode(SWDCK, PI_OUTPUT);
    
    gpioSetMode(SWDIO, PI_OUTPUT);

    initDevice(SWDCK, SWDIO);
    unsigned int data = readSWD(SWDCK, SWDIO, 0, 0);
    printf("Result = %08x\n", data);
    finishTransaction(SWDCK, SWDIO);
    return 0;
}
