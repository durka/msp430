#include "common.h"
#include "morse.h"
#include "nokia_lcd.h"

#define LED_PORT     P1
#define LED_PIN        p6
#define KEY_PORT     P1
#define KEY_PIN        p0
#define NOKIA_PORT   P1
#define NOKIA_DC       p1
#define NOKIA_SDIN     p2
#define NOKIA_SCLK     p4
#define NOKIA_SCE      p5
#define NOKIA_RESET    p7

static NokiaLCD nokia;

int main(void)
{
    WDTCTL = WDTPW + WDTHOLD;
    DCOCTL = 0;
    BCSCTL1 = CALBC1_1MHZ;
    DCOCTL = CALDCO_1MHZ;

    pinMode(LED_PORT, LED_PIN, OUTPUT);
    digitalWrite(LED_PORT, LED_PIN, LOW);

    Morse jim = morse(LED_PORT, LED_PIN, 150);
    lcd_init(&nokia, NOKIA_PORT, NOKIA_SCE, NOKIA_RESET, NOKIA_DC, NOKIA_SDIN, NOKIA_SCLK);

    lcd_clear(&nokia);
    lcd_gotoxy(&nokia, 0, 0);
    lcd_string(&nokia, "HI");
    sleep(10);

    int i = 0;
    while (1)
    {
        //PxOUT(LED_PORT) |= 1<<LED_PIN;
        //sleep(100);
        //PxOUT(LED_PORT) &= ~(1<<LED_PIN);
        //sleep(100);
        morse_str(&jim, "4");
        // -- ... .--. ....- ...-- -----    .-.. .- ..- -. -.-. .... .--. .- -..
        sleep(1000);
        lcd_itoa(&nokia, i++);
    }

    return 0;
}

