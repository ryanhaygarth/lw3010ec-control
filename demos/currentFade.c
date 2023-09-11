#include "../lw3010.h"
#include <stdio.h>
#include <modbus/modbus.h>
#include <errno.h>
#include <stdbool.h>
#include <unistd.h>

char commPort[] = "/dev/ttyUSB0";
int remoteId = 1;
int maxVoltage = 3000;
int maxCurrent = 175;

int main() {
    modbus_t *ctx = NULL;
    connect(&ctx, commPort, remoteId);

    writeCurrent(ctx, 0, maxCurrent);
    writeVoltage(ctx, 1400, maxVoltage);
    writeOutput(ctx, true);

    int i;
    for (i=0; i < maxCurrent;) {
        i = i + 1;
        writeCurrent(ctx, i, maxCurrent);
        if (i == maxCurrent) {
            i = 0;
        }
    }

    disconnect(ctx);
    return 0;
}