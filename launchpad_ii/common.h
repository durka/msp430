#ifndef _BURKA_COMMON_H_
#define _BURKA_COMMON_H_

#include <msp430g2553.h>

#define PxX(p,x) P##p##x
#define PxOUT(p) PxX(p,OUT)
#define PxDIR(p) PxX(p,DIR)
#define PxIN(p)  PxX(p,IN)
#define PxSEL(p) PxX(p,SEL)

typedef enum { P1=1, P2=2 }                                     Port;
typedef enum { p0=0, p1=1, p2=2, p3=3, p4=4, p5=5, p6=6, p7=7 } Pin;
typedef enum { LOW=0, HIGH=1 }                                  PinState;
typedef enum { INPUT, OUTPUT }                                  PinMode;

void sleep(unsigned long ms);
void digitalWrite(Port port, Pin pin, PinState state);
void pinMode(Port port, Pin pin, PinMode mode);

// TODO: MSP430 class, w/ utilities for watchdog, dco calib, etc? then we can track MS_TO_CYC

#endif // _BURKA_COMMON_H_

