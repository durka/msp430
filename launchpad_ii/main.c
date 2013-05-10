#include "common.h"
#include "morse.h"
#include "nokia_lcd.h"
#include "key.h"

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

int main(void)
{
    WDTCTL = WDTPW + WDTHOLD;
    DCOCTL = 0;
    BCSCTL1 = CALBC1_1MHZ;
    DCOCTL = CALDCO_1MHZ;

    pinMode(LED_PORT, LED_PIN, OUTPUT);
    digitalWrite(LED_PORT, LED_PIN, LOW);

    Morse jim;
    morse(&jim, LED_PORT, LED_PIN, 150);
    NokiaLCD nokia;
    lcd_init(&nokia, NOKIA_PORT, NOKIA_SCE, NOKIA_RESET, NOKIA_DC, NOKIA_SDIN, NOKIA_SCLK);
    Key board;
    key(&board, KEY_PORT, KEY_PIN);

    _BIS_SR(GIE);

    lcd_clear(&nokia);
    lcd_gotoxy(&nokia, 0, 0);
    lcd_string(&nokia, "HI");
    lcd_gotoxy(&nokia, 1, 1);
    sleep(10);

    int x;
    while (1)
    {
        //PxOUT(LED_PORT) |= 1<<LED_PIN;
        //sleep(100);
        //PxOUT(LED_PORT) &= ~(1<<LED_PIN);
        //sleep(100);
        //morse_str(&jim, "4");
        // -- ... .--. ....- ...-- -----    .-.. .- ..- -. -.-. .... .--. .- -..
        sleep(50);
        x = nokia.lcd_x;
        lcd_gotoxy(&nokia, 0, nokia.lcd_y);
        lcd_char(&nokia, '>');
        lcd_gotoxy(&nokia, x, nokia.lcd_y);
        switch (board.key)
        {
            case '\0':
                break;
                
            case '*':
                nokia.lcd_y++;
                if (nokia.lcd_y > LCD_Y/8)
                {
                    nokia.lcd_y = 1;
                }
                nokia.lcd_x = 1;
                lcd_gotoxy(&nokia, nokia.lcd_x, nokia.lcd_y);
                break;

            case '#':
                nokia.lcd_x--;
                if (nokia.lcd_x < 1)
                {
                    nokia.lcd_x = 1;
                }
                lcd_gotoxy(&nokia, nokia.lcd_x, nokia.lcd_y);
                break;

            default:
                lcd_char(&nokia, board.key);
                break;
        }
    }

    return 0;
}

