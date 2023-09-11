#ifndef LW3010EC_CONTROL
#define LW3010EC_CONTROL
#include <stdio.h>
#include <modbus/modbus.h>
#include <errno.h>
#include <stdbool.h>

void connect(modbus_t **ctx, char commPort[], int remoteId);
void disconnect(modbus_t *ctx);
void writeVoltage(modbus_t *ctx, int value, int maxVoltage);
void writeCurrent(modbus_t *ctx, int value, int maxCurrent);
void writeOutput(modbus_t *ctx, bool output);
//TODO - readOutputs
#endif