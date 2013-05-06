#include "morse.h"

Morse morse(Port port, Pin pin, unsigned long dot_ms)
{
    Morse this;
    this.port = port;
    this.pin = pin;
    this.dot_ms = dot_ms;
    pinMode(port, pin, OUTPUT);
    return this;
}

void morse_dot(const Morse* this)
{
    digitalWrite(this->port, this->pin, HIGH);
    sleep(this->dot_ms);
    digitalWrite(this->port, this->pin, LOW);
}

void morse_dash(const Morse* this)
{
    digitalWrite(this->port, this->pin, HIGH);
    sleep(this->dot_ms * 3);
    digitalWrite(this->port, this->pin, LOW);
}

static const char* morse_code[] = {
    ".-",    /* A */
    "-...",  /* B */
    "-.-.",  /* C */
    "-..",   /* D */
    ".",     /* E */
    "..-.",  /* F */
    "--.",   /* G */
    "....",  /* H */
    "..",    /* I */
    ".---",  /* J */
    "-.-",   /* K */
    ".-..",  /* L */
    "--",    /* M */
    "-.",    /* N */
    "---",   /* O */
    ".--.",  /* P */
    "--.-",  /* Q */
    ".-.",   /* R */
    "...",   /* S */
    "-",     /* T */
    "..-",   /* U */
    "...-",  /* V */
    ".--",   /* W */
    "-..-",  /* Y */
    "--..",  /* Z */
    "-----", /* 0 */
    ".----", /* 1 */
    "..---", /* 2 */
    "...--", /* 3 */
    "....-", /* 4 */
    ".....", /* 5 */
    "-....", /* 6 */
    "--...", /* 7 */
    "---..", /* 8 */
    "----.", /* 9 */
};

void morse_send(const Morse *this, const char *code)
{
    int i;

    for (i = 0; code[i]; ++i)
    {
        switch (code[i])
        {
            case '.':
                morse_dot(this);
                break;
            case '-':
                morse_dash(this);
                break;
        }
        sleep(this->dot_ms);
    }
}

void morse_char(const Morse *this, const char c)
{
    if (c >= 'A' && c <= 'Z')
	    morse_send(this, morse_code[c - 'A']);
    else if (c >= 'a' && c <= 'z')
	    morse_send(this, morse_code[c - 'a']);
    else if (c >= '0' && c <= '9')
	    morse_send(this, morse_code[c - '0' + 25]);

    sleep(this->dot_ms);
}

void morse_str(const Morse *this, const char *s)
{
    int i, j;

    for (i = 0; s[i]; ++i)
    {
        switch (s[i])
        {
            case ' ':
                sleep(this->dot_ms * 6);
                break;
            default: /* assuming A-Za-z0-9 */
                morse_char(this, s[i]);
                sleep(this->dot_ms * 3);
                break;
        }
    }
}

