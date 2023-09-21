#include <stdio.h>
#include <stdlib.h>
#include <modbus/modbus.h>
#include <errno.h>
#include <stdbool.h>
#include <unistd.h>

/*
Longwei LW-3010EC power supply controls
Written by Ryan Haygarth

-- VOLTAGES AND CURRENTS ARE OF TYPE INTEGER AND ARE MULTIPLIED BY 100 (10.5V = 1050) --

Sometimes the program will output an "Invalid data" error, this seems to happen mainly the first time the program
is run after the power supply is turned on.

Register list
0x1000, voltage write
0x1001, current write
0x1002, voltage read
0x1003, current read
0x1004, output read
0x1005, constant current or constant voltage (0 = cc, 1 = cv)
0x1006, output write
*/

// Only change these if they are different for your power supply
const int MAX_VOLTAGE = 3000; 
const int MAX_CURRENT = 1000; 

/*
connect()
Inputs
    **ctx = a pointer to the ctx pointer
    commPort[] = directory for the comm port ("/dev/ttyUSB0")
    remoteId = remote id of the power supply (1)
Description
    Connects to the power supply at (commPort)
Example
    connect(&ctx, "/dev/ttyUSB0", 1);
*/

void connect(modbus_t **ctx, char commPort[], int remoteId) {
    *ctx = modbus_new_rtu(commPort, 9600, 'N', 8, 1); // from the manual: baud: 9600, parity: none, data: 8 bits, stop bits: 1 
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
    modbus_set_response_timeout(*ctx, 0, 200000);
    printf("connected\n");
}

/*
disconnect()
Inputs
    *ctx = ctx
Description
    Disconnects the ctx instance
Example
    disconnect(ctx);
*/

void disconnect(modbus_t *ctx) {
    modbus_close(ctx);
    modbus_free(ctx);
    printf("disconnected\n");
}

/*
readValue()
Inputs
    *ctx = ctx
    regNumber = number of register (last number of address in register list above (eg 3 = 0x1003))
Description
    returns the value at address 0x100(regNumber)
Example
    readValue(ctx, 2);
*/

int readValue(modbus_t *ctx, int regNumber) {
    usleep(200000);
    uint16_t registers[12];
    if (modbus_read_registers(ctx, 0x1000, 6, registers) == -1) {
        fprintf(stderr, "error reading output - %s\n", modbus_strerror(errno));
        return -1;
    }
    return *(registers + regNumber);
}

/*
writeVoltage()
Inputs
    *ctx = ctx
    value = desired voltage
Description
    changes the voltage to value
Example
    writeVoltage(ctx, 3000);
*/

void writeVoltage(modbus_t *ctx, int value) {
    usleep(200000);
    if (value > MAX_VOLTAGE) {
        fprintf(stderr, "error setting voltage - %d is above maximum (%d)\n", value, MAX_VOLTAGE);
    } else if (modbus_write_register(ctx, 0x1000, value) == -1) {
        int possibleError = errno;
        usleep(200000); 
        // This usleep seems to prevent a second invalid data error and let it check if it wrote correctly,
        // As normally it gives the error but it has written the value correctly
        // Nested if to print correct errno
        if (readValue(ctx, 0) != value) {
            fprintf(stderr, "error setting output - %s\n", modbus_strerror(possibleError));
            disconnect(ctx);
            fprintf(stderr, "exiting\n");
            exit(EXIT_FAILURE);
        }
        // This is only here to let the user know what the program could be running slow as
        // Sometimes the invalid data error keeps repeating for every write until the program is terminated
        printf("ignoring \"%s\" error\n", modbus_strerror(possibleError));
    }
}

/*
writeCurrent()
Inputs
    *ctx = ctx
    value = desired current
Description
    changes the current to value
Example
    writeCurrent(ctx, 3000);
*/

void writeCurrent(modbus_t *ctx, int value) {
    usleep(200000);
    if (value > MAX_CURRENT) {
        fprintf(stderr, "error setting current - %d is above maximum (%d)\n", value, MAX_CURRENT);
    } else if (modbus_write_register(ctx, 0x1001, value) == -1) {
        int possibleError = errno;
        usleep(200000);
        // nested if to print correct errno
        if (readValue(ctx, 1) != value) {
            fprintf(stderr, "error setting current %s\n", modbus_strerror(possibleError));
            disconnect(ctx);
            fprintf(stderr, "exiting\n");
            exit(EXIT_FAILURE);
        }
        printf("ignoring \"%s\" error\n", modbus_strerror(possibleError));
    }
}

/*
writeOutput()
Inputs
    *ctx = ctx
    output = desired output status
Description
    changes the output status to the value of output
Example
    writeOutput(ctx, true);
*/

void writeOutput(modbus_t *ctx, bool output) {
    usleep(200000);
    if (modbus_write_register(ctx, 0x1006, output) == -1) {
        int possibleError = errno;
        usleep(200000);
        // nested if to print correct errno
        if (readValue(ctx, 6) != output) {
            fprintf(stderr, "error setting output - %s\n", modbus_strerror(possibleError));
            disconnect(ctx);
            fprintf(stderr, "exiting\n");
            exit(EXIT_FAILURE);
        }
        printf("ignoring \"%s\" error\n", modbus_strerror(possibleError));
    }
}

/*
readOutputs()
Inputs
    *ctx = ctx
    *outputVoltage = pointer to output voltage variable
    *outputCurrent = pointer to output current variable
    *outputStatus = pointer to output status variable
Description
    updates the output variables
Example
    readOutputs(ctx, &outputVoltage, &outputCurrent, &outputStatus);
*/

void readOutputs(modbus_t *ctx, int *outputVoltage, int *outputCurrent, bool *outputStatus) {
    usleep(200000);
    uint16_t registers[12];
    if (modbus_read_registers(ctx, 0x1000, 6, registers) == -1) {
        fprintf(stderr, "error reading output - %s\n", modbus_strerror(errno));
        disconnect(ctx);
        exit(EXIT_FAILURE);
    }
    *outputVoltage = *(registers + 2);
    *outputCurrent = *(registers + 3);
    *outputStatus = *(registers + 4);
}
