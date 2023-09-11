#include "../lw3010.h"
#include <stdio.h>
#include <modbus/modbus.h>
#include <errno.h>
#include <stdbool.h>
#include <unistd.h>

char commPort[] = "/dev/ttyUSB0";
int remoteId = 1;
int maxVoltage = 3000;
int maxCurrent = 1000;

int main() {
    modbus_t *ctx = NULL;
    connect(&ctx, commPort, remoteId);
    
    writeOutput(ctx, true);
    writeCurrent(ctx, maxCurrent, maxCurrent);

    int i;
    for (i=0; i < maxVoltage;) {
        i = i + 20;
        writeVoltage(ctx, i, maxVoltage);
        usleep(50000);
        if (i == 1800) {
            i = 0;
        }
    }

    disconnect(ctx);
    return 0;
}