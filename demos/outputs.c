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
int maxVoltage = 3000;
int maxCurrent = 1000;
int outputVoltage = 0;
int outputCurrent = 0;
bool outputStatus = false;

void exitDisconnect(int signum) {
    disconnect(ctx);
    exit(EXIT_SUCCESS);
}

int main() {
    connect(&ctx, commPort, remoteId);
    signal(SIGINT, exitDisconnect);

    readOutputs(ctx, &outputVoltage, &outputCurrent, &outputStatus);

    printf("v = %d\n", outputVoltage);
    printf("c = %d\n", outputCurrent);
    printf("o = %d\n", outputStatus);

    disconnect(ctx);
    return 0;
}