#include "../lw3010.h"
#include <stdio.h>
#include <modbus/modbus.h>
#include <errno.h>
#include <stdbool.h>
#include <unistd.h>

char commPort[] = "/dev/ttyUSB0";
int remoteId = 1;
int maxVoltage = 1400;
int maxCurrent = 180;
int minCurrent = 50;

int main() {
    modbus_t *ctx = NULL;
    connect(&ctx, commPort, remoteId);

    writeCurrent(ctx, minCurrent, maxCurrent);
    writeVoltage(ctx, maxVoltage, maxVoltage);
    writeOutput(ctx, true);

    // need to check if voltage is above maximum, as currently the power supply disreguards the maximum voltage setting

    int i;
    for (i=minCurrent; i <= maxCurrent;) {
        i = i + 5;
        writeCurrent(ctx, i, maxCurrent);
        if (i >= maxCurrent) {
            i = minCurrent;
        }
    }

    disconnect(ctx);
    return 0;
}