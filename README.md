# lw3010ec-control
Power Supply Control for the Longwei LW3010EC Power Supply Written in C

## Contents

- [About](#about)
- [Usage](#usage)
- [Expected Inputs](#expected-inputs)
- [Building](#building)
- [Todo](https://github.com/ryanhaygarth/lw3010ec-control/blob/main/TODO.md)

### About

This program was written to control a Longwei LW3010EC programmable power supply using the modbus protocol. I wrote this program to try and learn C, so if you have any suggestions or other ways to do things, please let me know. The program uses the libmodbus library to communicate with the power supply. There are multiple demo files included to show what the program can do.

### Usage

First, you need to install the libmodbus library if not installed already. Instructions are on the [Libmodbus](https://libmodbus.org/getting_started/) website.  
Once installed, you can include the header file into any file to use its functions in your programs.  
```#include "lw3010.h"```  
Make sure that the path to the header file is correct, for example in the demo directory the path is ```"../src/lw3010.h"``` instead.  

### Expected Inputs

```commPort[]``` is the directory of where the usb device is (example is /dev/ttyUSB0)  
```remoteId``` is the remote id of the device (default is 1)  
**VOLTAGES AND CURRENTS ARE OF TYPE INTEGER AND ARE MULTIPLIED BY 100 (10.5V = 1050)**  

### Example Function Usage

These 3 lines are required to connect to the power supply and disconnect after.  
```
modbus_t *ctx = NULL;
connect(&ctx, "/dev/ttyUSB0", 1);

// do something here

disconnect(ctx);
```
You can then write to the power supply using these functions.  
```
writeVoltage(ctx, 1400); // set the voltage to 14V
writeCurrent(ctx, 1000); // set the current limit to 10A
writeOutput(ctx, true); // turn the output on
```  
Read [demos/template.c](https://github.com/ryanhaygarth/lw3010ec-control/blob/main/demos/template.c) to see how to make sure disconnect() is called when the program is exited with SIGINT (for example if the program runs in a loop, etc.)  

### Building

To build something from this project, you need to link the modbus library when compiling.  

An example to compile the fade.c file would be  
```gcc src/lw3010.c demos/fade.c -o fade -lmodbus```  
