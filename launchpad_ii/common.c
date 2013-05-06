#include "common.h"

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

