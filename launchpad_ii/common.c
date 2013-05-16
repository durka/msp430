#include "common.h"

void initProcessor()
{
    WDTCTL = WDTPW + WDTHOLD;
    DCOCTL = 0;
    BCSCTL1 = CALBC1_1MHZ;
    DCOCTL = CALDCO_1MHZ;
    FCTL2 = FWKEY + FSSEL0 + FN1;             // MCLK/3 for Flash Timing Generator
}

#define MS_TO_CYC (unsigned long)1000
inline void sleep(unsigned long ms)
{
	unsigned long j;
	for (j = 0; j < ms; ++j)
	{
		__delay_cycles(MS_TO_CYC);
	}
}

#define pcase(p) \
	case p: \
		if (state == HIGH) PxOUT(p) |= mask; \
		else PxOUT(p) &= ~mask; \
		break
inline void digitalWrite(Port port, Pin pin, PinState state)
{
	unsigned char mask = 1 << pin;
	switch (port)
	{
		pcase(1);
		pcase(2);
	}
}
#undef pcase

#define pcase(p) \
	case p: \
		if (mode == OUTPUT) PxDIR(p) |= mask; \
		else PxDIR(p) &= ~mask; \
		break
inline void pinMode(Port port, Pin pin, PinMode mode)
{
	unsigned char mask = 1 << pin;
	switch (port)
	{
		pcase(1);
		pcase(2);
	}
}
#undef pcase

void shiftOut(Port dataport, Pin datapin, Port clockport, Pin clockpin, BitOrder order, byte data)
{
    byte i;

    for (i = 0; i < 8; ++i)            // loop through all 8 bits
    {
        if (order == LSBFIRST)            // LSB first?
        {
            digitalWrite(dataport, datapin, !!(data & (1 << i)));
            // shift out a bit
        }
        else
        {
            digitalWrite(dataport, datapin, !!(data & (1 << (7 - i))));
        }
     
        digitalWrite(clockport, clockpin, HIGH); // pulse the clock
        digitalWrite(clockport, clockpin, LOW);
    }
}

void flashWrite(byte *source, byte page, unsigned short offset, unsigned short len)
{
    byte *flashptr;
    unsigned int i;

    flashptr = (byte*)(0xFFFF - (page+1)*512 + 1);
}

void flashRead(byte *dest, byte page, unsigned short offset, unsigned short len)
{
}

