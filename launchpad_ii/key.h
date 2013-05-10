#ifndef _BURKA_KEY_H_
#define _BURKA_KEY_H_

#include "common.h"

typedef struct
{
    Port port;
    Pin pin;

    byte val;
    char key;
} Key;

void key(Key *this, Port port, Pin pin);

#endif // _BURKA_KEY_H_

