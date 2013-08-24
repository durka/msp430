#include "common.h"
#include "morse.h"
#include "nokia_lcd.h"
#include "key.h"
#include "serial.h"

// P1.1 and P1.2 are reserved for the serial port
#define LED_PORT     P1
#define LED_PIN        p6
#define KEY_PORT     P1
#define KEY_PIN        p0
#define NOKIA_PORT   P1
#define NOKIA_DC       p3
#define NOKIA_SDIN     p6
#define NOKIA_SCLK     p4
#define NOKIA_SCE      p5
#define NOKIA_RESET    p7
#define NOKIA_PINS NOKIA_SCE, NOKIA_RESET, NOKIA_DC, NOKIA_SDIN, NOKIA_SCLK

void prompt(NokiaLCD *nokia, Key *board)
{
    char c;
    while ((c = key_get(board)) != EOF)
    {
        switch (c)
        {
            case '\0':
                break;

            case '*':
                nokia->s.y++;
                if (nokia->s.y >= nokia->s.height)
                {
                    nokia->s.y = 1;
                }
                lcd_gotoxy(nokia, 1, nokia->s.y);
                break;

            case '#':
                lcd_clear(nokia);
                lcd_drawbox(nokia);
                lcd_gotoxy(nokia, 0, 0);
                lcd_string(nokia, "HI");
                lcd_gotoxy(nokia, 0, 1);
                lcd_char(nokia, '>');
                break;

            default:
                {
                    int y = nokia->s.y;
                    lcd_char(nokia, c);
                    if (nokia->s.y != y)
                    {
                        lcd_char(nokia, '>');
                    }
                    break;
                }
        }
    }
}

void serial_echo(NokiaLCD *nokia, Serial *cheerios)
{
    
}

int main(void)
{
    initProcessor();

    pinMode(LED_PORT, LED_PIN, OUTPUT);
    digitalWrite(LED_PORT, LED_PIN, LOW);

    Morse jim;
    morse(&jim, LED_PORT, LED_PIN, 150);
    NokiaLCD nokia;
    lcd_init(&nokia, NOKIA_PORT, NOKIA_PINS, LCD_X/4, 0, LCD_X*3/4, LCD_Y/8/2);
    Key board;
    key(&board, KEY_PORT, KEY_PIN);
    Serial cheerios;
    serial(&cheerios);

    _BIS_SR(GIE);

    lcd_drawbox(&nokia);
    lcd_gotoxy(&nokia, 0, 0);
    lcd_string(&nokia, "HI");
    lcd_gotoxy(&nokia, 0, 1);
    lcd_char(&nokia, '>');

    while (1)
    {
        //PxOUT(LED_PORT) |= 1<<LED_PIN;
        //sleep(100);
        //PxOUT(LED_PORT) &= ~(1<<LED_PIN);
        //sleep(100);
        //morse_str(&jim, "4");
        // -- ... .--. ....- ...-- -----    .-.. .- ..- -. -.-. .... .--. .- -..
        sleep(100);
        prompt(&nokia, &key);
        //serial_echo(&nokia, &cheerios);
    }

    return 0;
}

