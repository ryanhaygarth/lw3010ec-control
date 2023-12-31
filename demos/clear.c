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
int maxCurrent = 1000;

void exitDisconnect(int signum) {
    disconnect(ctx);
    exit(EXIT_SUCCESS);
}

int main() {
    connect(&ctx, commPort, remoteId);
    signal(SIGINT, exitDisconnect);

    writeVoltage(ctx, 0);
    writeCurrent(ctx, maxCurrent);
    writeOutput(ctx, false);

    disconnect(ctx);
    return 0;
}