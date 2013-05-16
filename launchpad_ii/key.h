#ifndef _BURKA_KEY_H_
#define _BURKA_KEY_H_

#include "common.h"

#define KEYBUF 50
#define EOF -1

typedef struct
{
    Port port;
    Pin pin;

    unsigned short i, j;
    char key[KEYBUF];
} Key;

void key(Key *this, Port port, Pin pin);
int key_get(Key *this);

#endif // _BURKA_KEY_H_

