#include "../lw3010.h"
#include <stdio.h>
#include <modbus/modbus.h>
#include <errno.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdlib.h>
#include <getopt.h>
#include <limits.h>
#include <string.h>

modbus_t *ctx = NULL;
char commPort[] = "/dev/ttyUSB0";
int remoteId = 1;
int maxVoltage = 3000;
int maxCurrent = 1000;
int option = 0;
int setVoltage = 0;
int setCurrent = 0;
bool setOutput = 0;
bool changeVoltage = false;
bool changeCurrent = false;
bool changeOutput = false;


void exitDisconnect(int signum) {
    disconnect(ctx);
    exit(EXIT_SUCCESS);
}

int main(int argc, char **argv) {
    while (( option = getopt(argc, argv, "v:c:o:h")) != -1 ) {
        switch (option) {

            case 'v':
                setVoltage = strtol(optarg, NULL, 10);
                changeVoltage = true;
                break;
            case 'c':
                setCurrent = strtol(optarg, NULL, 10);
                changeCurrent = true;
                break;
            case 'o': 
                if (strcmp(optarg, "on") == 0) {
                    setOutput = true;
                    changeOutput = true;
                } else if (strcmp(optarg, "off") == 0) {
                    setOutput = false;
                    changeOutput = true;
                } else {
                    printf("-o [on/off]\n");
                }
                break;
            case 'h':
                printf("USAGE : set -v voltage -c current -o output\n");
                printf("EXAMPLE: set -v 1200 -c 1000 -o on");
                break;
            default:
                printf("unknown option %c \n", optopt);
                return 1;
        }
    }

    connect(&ctx, commPort, remoteId);
    signal(SIGINT, exitDisconnect);

    if (changeVoltage) { writeVoltage(ctx, setVoltage, maxVoltage); }
    if (changeCurrent) { writeCurrent(ctx, setCurrent, maxCurrent); }
    if (changeOutput) { writeOutput(ctx, setOutput); }
    disconnect(ctx);
    return 0;
}