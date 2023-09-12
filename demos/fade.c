#include "../lw3010.h"
#include <stdio.h>
#include <modbus/modbus.h>
#include <errno.h>
#include <stdbool.h>
#include <unistd.h>

char commPort[] = "/dev/ttyUSB0";
int remoteId = 1;
int maxVoltage = 1300;
int maxCurrent = 1000;
int minVoltage = 300;

int main() {
    modbus_t *ctx = NULL;
    connect(&ctx, commPort, remoteId);
    
    writeOutput(ctx, true);
    writeCurrent(ctx, maxCurrent, maxCurrent);

    // loop from minimum voltage to max voltage in increments of 0.5V
    int i;
    for (i=minVoltage; i < maxVoltage;) {
        i = i + 50;
        writeVoltage(ctx, i, maxVoltage);
        if (i >= maxVoltage) {
            i = minVoltage;
        }
    }

    disconnect(ctx);
    return 0;
}
