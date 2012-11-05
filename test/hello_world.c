#include <msp430g2553.h>

#define PxX(p,x) P##p##x
#define PxOUT(p) PxX(p,OUT)
#define PxDIR(p) PxX(p,DIR)
#define PxIN(p)  PxX(p,IN)
#define PxSEL(p) PxX(p,SEL)

#define LED_PORT 1
#define LED_PIN  6

#define DOT_MS 50

#define MS_TO_CYC (unsigned long)1000
#define sleep(ms) __delay_cycles(ms*MS_TO_CYC)

void dot()
{
    PxOUT(LED_PORT) |= 1<<LED_PIN;
    sleep(DOT_MS);
    PxOUT(LED_PORT) &= ~(1<<LED_PIN);
}

void dash()
{
    PxOUT(LED_PORT) |= 1<<LED_PIN;
    sleep(DOT_MS * 3);
    PxOUT(LED_PORT) &= ~(1<<LED_PIN);
}

char* morse_code[] = {
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
};

void morse_send(char *code)
{
    int i;

    for (i = 0; code[i]; ++i)
    {
        switch (code[i])
        {
            case '.':
                dot();
                break;
            case '-':
                dash();
                break;
        }
    }
}

void morse_char(char c)
{
    if (c >= 'A' && c <= 'Z')
        morse_send(morse_code[c - 'A']);
    else if (c >= 'a' && c <= 'z')
        morse_send(morse_code[c - 'a']);

    sleep(DOT_MS);
}

void morse_str(char *s)
{
    int i;

    for (i = 0; s[i]; ++i)
    {
        switch (s[i])
        {
            case ' ':
                sleep(DOT_MS * 6);
                break;
            default: /* assuming A-Za-z */
                morse_char(s[i]);
                sleep(DOT_MS * 3);
                break;
        }
    }
}

int main(void)
{
    WDTCTL = WDTPW + WDTHOLD;
    DCOCTL = 0;
    BCSCTL1 = CALBC1_1MHZ;
    DCOCTL = CALDCO_1MHZ;

    PxDIR(LED_PORT) |= 1<<LED_PIN;
    PxOUT(LED_PORT) &= ~(1<<LED_PIN);

    while (1)
    {
        //PxOUT(LED_PORT) |= 1<<LED_PIN;
        //sleep(100);
        //PxOUT(LED_PORT) &= ~(1<<LED_PIN);
        //sleep(100);
        morse_str("SOS");
        sleep(1000);
    }

    return 0;
}
