#ifndef _BURKA_MORSE_H_
#define _BURKA_MORSE_H_

#include "common.h"

typedef struct
{
    Port port;
    Pin pin;
    unsigned long dot_ms;
} Morse;

void morse(Morse* this, Port port, Pin pin, unsigned long dot_ms);
void morse_dot(const Morse*);
void morse_dash(const Morse*);
void morse_send(const Morse*, const char*);
void morse_char(const Morse*, const char);
void morse_str(const Morse*, const char*);

#endif // _BURKA_MORSE_H_

