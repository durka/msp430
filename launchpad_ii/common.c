#include "common.h"

void initProcessor()
{
    WDTCTL = WDTPW + WDTHOLD;
    DCOCTL = 0;
    BCSCTL1 = CALBC1_1MHZ;
    DCOCTL = CALDCO_1MHZ;
    FCTL2 = FWKEY + FSSEL0 + FN1;             // MCLK/3 for Flash Timing Generator

    flashClear(31);
    flashClear(30);
    flashClear(29);
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

// INFO bank: page should be 'A', 'B', 'C', 'D'
// ROM bank: page should be 0-31
byte* get_flashptr(FlashBank bank, byte page)
{
    switch (bank)
    {
        case INFO:
            return (byte*)(0x10FF - (page-'A'+1)*64 + 1);

        case ROM:
            return (byte*)(0xFFFF - (page+1)*512 + 1);
    }
}

void flashClear(Flashbank bank, byte page)
{
    byte *flashptr = get_flashptr(page);

    FCTL1 = FWKEY + ERASE;                    // Set Erase bit
    FCTL3 = FWKEY;                            // Clear Lock bit
    *flashptr = 0;                           // Dummy write to erase Flash segment

    FCTL1 = FWKEY;                            // Clear WRT bit
    FCTL3 = FWKEY + LOCK;                     // Set LOCK bit
}

void flashWrite(const byte *source, FlashBank bank, byte page, unsigned short offset, unsigned short len)
{
    unsigned int i;
    byte *flashptr = get_flashptr(bank, page);

    flashClear(page);

    FCTL1 = FWKEY + WRT;                     // Set Write bit
    for (i = 0; i < (bank == INFO ? 64 : 512); ++i)
    {
        flashptr[i] = source[i];
    }
    FCTL1 = FWKEY;
    FCTL3 = FWKEY + LOCK;
}

void flashRead(byte *dest, FlashBank bank, byte page, unsigned short offset, unsigned short len)
{
    unsigned int i;
    byte *flashptr = get_flashptr(bank, page) + offset;

    for (i = 0; i < len; ++i)
    {
        dest[i] = flashptr[i];
    }

}

void flashCopy(const byte *source, FlashBank bank, byte pagefrom, byte pageto, unsigned short offset, unsigned short len)
{
    unsigned int i;
    byte *fromptr = get_flashptr(bank, pagefrom);
    byte *toptr = get_flashptr(bank, pageto);

    flashClear(pageto);

    FCTL1 = FWKEY + WRT;
    for (i = 0; i < offset; ++i)
    {
        toptr[i] = fromptr[i];
    }
    for (; i < offset+len; ++i)
    {
        toptr[i] = source[i-offset];
    }
    for (; i < (bank == INFO ? 64 : 512); ++i)
    {
        toptr[i] = fromptr[i];
    }
    FCTL1 = FWKEY;
    FCTL3 = FWKEY + LOCK;
}

