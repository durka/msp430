#include <signal.h>
#include <string.h>
#include "key.h"

static Key *K = NULL;

void key(Key *this, Port port, Pin pin)
{
    K = this; // TODO singleton/multiple keyboards

    // TODO interrupt pin triggers ADC run?

    // set up ADC interrupt
    ADC10CTL1 = ADC10DIV_3 + SHS_1 + CONSEQ_2 + (pin << 12);                      // input A0 (I happen to know that INCH_i == 0xi000)
    ADC10CTL0 = SREF_0 + ADC10SHT_3 + ADC10ON + REFON + ADC10IE; // ADC10ON, interrupt enabled
    ADC10AE0 |= 1<<pin;                         // ADC option select

    // set up timer A
    TACCR0 = 30;
    TACCTL1 = OUTMOD_4;
    TACTL = TASSEL_2 + MC_2;

    this->i = this->j = 0;
    memset(this->key, 0, sizeof this->key);

    // start the first conversion
    ADC10CTL0 |= ENC;
}

int key_get(Key *this)
{
    if (this->i == this->j) return EOF;

    char c = this->key[this->i++];
    if (this->i >= KEYBUF) this->i = 0;
    return c;
}

#include "keymap.inc"

// TODO central interrupt registry in common.c
interrupt(ADC10_VECTOR) ADC10_ISR(void)
{
    static unsigned int count = 0;
    static unsigned char prev = '\0';
    
    char c = table[ADC10MEM];

    if (K)
    {
        if (    (ADC10MEM > 10) // no key is pressed
             && !((c == prev) && (count < 3)) // pause before key repeat
           )
        {
            K->key[K->j++] = c;
            if (K->j >= KEYBUF) K->j = 0;
        }
    }

    if (prev != c)
    {
        count = 0;
        prev = c;
    }
    else
    {
        ++count;
    }
}

