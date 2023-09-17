#include "../lw3010.h"
#include <stdio.h>
#include <modbus/modbus.h>
#include <errno.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

modbus_t *ctx = NULL;
char commPort[] = "/dev/ttyUSB0";
int remoteId = 1;
int maxVoltage = 1300;
int maxCurrent = 1000;
int minVoltage = 300;

void exitDisconnect(int signum) {
    disconnect(ctx);
    exit(EXIT_SUCCESS);
}

int main() {
    connect(&ctx, commPort, remoteId);
    signal(SIGINT, exitDisconnect);
    
    writeOutput(ctx, true);
    writeCurrent(ctx, maxCurrent, maxCurrent);

    // loop from minimum voltage to max voltage in increments of 0.5V
    int i;
    for (i=minVoltage; i <= maxVoltage;) {
        i = i + 50;
        writeVoltage(ctx, i, maxVoltage);
        if (i >= maxVoltage) {
            i = minVoltage;
        }
    }

    disconnect(ctx);
    return 0;
}
