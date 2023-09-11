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

    // do something here

    disconnect(ctx);
    return 0;
}