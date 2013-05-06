#include "common.h"
#include "morse.h"

#define LED_PORT P1
#define LED_PIN  p6

int main(void)
{
    WDTCTL = WDTPW + WDTHOLD;
    DCOCTL = 0;
    BCSCTL1 = CALBC1_1MHZ;
    DCOCTL = CALDCO_1MHZ;

    pinMode(LED_PORT, LED_PIN, OUTPUT);
    digitalWrite(LED_PORT, LED_PIN, LOW);

    Morse jim = morse(LED_PORT, LED_PIN, 150);

    while (1)
    {
        //PxOUT(LED_PORT) |= 1<<LED_PIN;
        //sleep(100);
        //PxOUT(LED_PORT) &= ~(1<<LED_PIN);
        //sleep(100);
        morse_str(&jim, "MSP430 Launchpad");
	// -- ... .--. ....- ...-- -----    .-.. .- ..- -. -.-. .... .--. .- -..
	sleep(1000);
    }

    return 0;
}

