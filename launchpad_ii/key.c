#include <signal.h>
#include "key.h"

static Key *the_key = NULL;

void key(Key *this, Port port, Pin pin)
{
    the_key = this; // TODO singleton

    // set up ADC interrupt
    ADC10CTL0 = ADC10SHT_2 + ADC10ON + ADC10IE; // ADC10ON, interrupt enabled
    ADC10CTL1 = pin << 12;                      // input A0 (I happen to know that INCH_i == 0xi000)
    ADC10AE0 |= 1<<pin;                         // ADC option select

    this.val = 0;
    this.key = '\0';
    return this;
}

// TODO central interrupt registry in common.c
interrupt(ADC10_VECTOR) ADC10_ISR(void)
{
    // process interrupt
    if (the_key)
    {
        the_key->val = ADC10MEM;
    }
}

