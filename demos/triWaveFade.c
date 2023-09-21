#include "../src/lw3010.h"
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
int maxVoltage = 1100;
int maxCurrent = 1000;
int minVoltage = 300;
int i = 300;
bool ascending = true;

void exitDisconnect(int signum) {
    disconnect(ctx);
    exit(EXIT_SUCCESS);
}

int main() {
    connect(&ctx, commPort, remoteId);
    signal(SIGINT, exitDisconnect);

    writeVoltage(ctx, minVoltage);
    writeCurrent(ctx, maxCurrent);
    writeOutput(ctx, true);

    while (true) {
        if (i == maxVoltage) {
            ascending = false;
        } else if (i == minVoltage) {
            ascending = true;
        }

        if (ascending == true) {
            i += 25;
        } else {
            i -= 25;
        }
        writeVoltage(ctx, i);
    }

    disconnect(ctx);
    return 0;
}