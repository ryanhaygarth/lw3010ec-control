#ifndef LW3010EC_CONTROL
#define LW3010EC_CONTROL
#include <stdio.h>
#include <modbus/modbus.h>
#include <errno.h>
#include <stdbool.h>

void connect(modbus_t **ctx, char commPort[], int remoteId);
void disconnect(modbus_t *ctx);
int readValue(modbus_t *ctx, int regNumber);
void writeVoltage(modbus_t *ctx, int value);
void writeCurrent(modbus_t *ctx, int value);
void writeOutput(modbus_t *ctx, bool output);
void readOutputs(modbus_t *ctx, int *outputVoltage, int *outputCurrent, bool *outputStatus);
#endif