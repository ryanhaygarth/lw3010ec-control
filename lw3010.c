#include <stdio.h>
#include <stdlib.h>
#include <modbus/modbus.h>
#include <errno.h>
#include <stdbool.h>
#include <unistd.h>

/*
Longwei LW-3010EC power supply controls
Written by Ryan Haygarth

Expected inputs:
commPort = comm port (example = /dev/ttyUSB0)
remoteId = remote id of power supply (default is 1)
-- VOLTAGES AND CURRENTS ARE OF TYPE INTEGER AND ARE MULTIPLIED BY 100 (10.5V = 1050) --
maxVoltage (default is 3000)
maxCurrent (default is 1000)

usleep statements are necessary otherwise modbus will give "invalid data" errors
I cannot find the cause of the "invalid data" errors, it seems they appear out of nowhere, however
almost all the time it seems that it works correctly but still gives that error?
*/

void connect(modbus_t **ctx, char commPort[], int remoteId) {
    *ctx = modbus_new_rtu(commPort, 9600, 'N', 8, 1); // from the manual: baud: 9600, parity: none, data: 8 bits, stop bits: 1 
    // commPort is the location of the device, example /dev/ttyUSB0
    if (*ctx == NULL) {
        fprintf(stderr, "unable to create new modbus instance on device %d: %s\n", remoteId, modbus_strerror(errno));
        exit(EXIT_FAILURE);
    }
    usleep(200000);
    if (modbus_connect(*ctx) == -1) {
        fprintf(stderr, "unable to connect to device %d: %s\n", remoteId, modbus_strerror(errno));
        modbus_free(*ctx);
        exit(EXIT_FAILURE);
    }
    usleep(200000);
    if (modbus_set_slave(*ctx, remoteId) == -1) {
        fprintf(stderr, "unable to set device %d as slave: %s\n", remoteId, modbus_strerror(errno));
        modbus_free(*ctx);
        printf("disconnected\n");
        exit(EXIT_FAILURE);
    }
    usleep(200000);
    printf("connected\n");
}

void disconnect(modbus_t *ctx) {
    modbus_close(ctx);
    modbus_free(ctx);
    printf("disconnected\n");
}

void writeVoltage(modbus_t *ctx, int value, int maxVoltage) {
    usleep(200000);
    if (value > maxVoltage) {
        fprintf(stderr, "error setting maxVoltage - %d is above maximum\n", value);
    } else if (modbus_write_register(ctx, 0x1000, value) == -1) {
        fprintf(stderr, "error setting maxVoltage - %s\n", modbus_strerror(errno));
        // if write_register fails, keep going
    }
}

void writeCurrent(modbus_t *ctx, int value, int maxCurrent) {
    usleep(200000);
    if (value > maxCurrent) {
        fprintf(stderr, "error setting maxCurrent - %d is above maximum\n", value);
    } else if (modbus_write_register(ctx, 0x1001, value) == -1) {
        fprintf(stderr, "error setting maxCurrent - %s\n", modbus_strerror(errno));
        // if write_register fails, keep going
    }
}

void writeOutput(modbus_t *ctx, bool output) {
    usleep(200000);
    if (modbus_write_register(ctx, 0x1006, output) == -1) {
        fprintf(stderr, "error setting output - %s\n", modbus_strerror(errno));
        // if write_register fails, keep going
    }
}

int readOutputs() {
    /*
    TODO
    voltage read register = 0x1002
    current read register = 0x1003
    output read register = 0x1004
    */
}
