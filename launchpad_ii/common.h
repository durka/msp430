#ifndef _BURKA_COMMON_H_
#define _BURKA_COMMON_H_

#include <msp430g2553.h>
#include <stdlib.h>

#define PxX(p,x) P##p##x
#define PxOUT(p) PxX(p,OUT)
#define PxDIR(p) PxX(p,DIR)
#define PxIN(p)  PxX(p,IN)
#define PxSEL(p) PxX(p,SEL)

// useful datatypes
typedef unsigned char byte;
typedef unsigned char bool;
#define true 1
#define false 0

typedef enum { P1=1, P2=2 }                                     Port;
typedef enum { p0=0, p1=1, p2=2, p3=3, p4=4, p5=5, p6=6, p7=7 } Pin;
typedef enum { LOW=0, HIGH=1 }                                  PinState;
typedef enum { INPUT, OUTPUT }                                  PinMode;
typedef enum { MSBFIRST, LSBFIRST }                             BitOrder;

void initProcessor(void);
void sleep(unsigned long ms);
void digitalWrite(Port port, Pin pin, PinState state);
void pinMode(Port port, Pin pin, PinMode mode);
void shiftOut(Port dataport, Pin datapin, Port clockport, Pin clockpin, BitOrder order, byte data);

// TODO: MSP430 class, w/ utilities for watchdog, dco calib, etc? then we can track MS_TO_CYC

#endif // _BURKA_COMMON_H_

