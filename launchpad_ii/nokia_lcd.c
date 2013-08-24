/*
* Library for Nokia 5110 LCD with PCD8544 controller
*
* Ported from Arduino sample code and enhanced
*
* Alex Burka
* Katherine Bertaut
* November 2010
*/

/*
* This Code has extra features
* including a XY positioning function on Display
* and a Line Draw function on Nokia 3310 LCD
* It is modded from the original
* http://www.arduino.cc/playground/Code/PCD8544
*/
// Mods by Jim Park
// jim(^dOt^)buzz(^aT^)gmail(^dOt^)com
// hope it works for you

#include <string.h>        // memset
#include "nokia_lcd.h"     // library header
#include "common.h"           // utility functions/macros

// values for data/command pin
#define LCD_C     LOW
#define LCD_D     HIGH

#define LCD_CMD   0  // same as LCD_C

// some private function prototypes
void lcd_raw_clear(NokiaLCD *this);
void lcd_raw_write(NokiaLCD *this, byte dc, byte data);
void lcd_raw_gotoxy(NokiaLCD *this, int xx, int yy);
//void lcd_save(NokiaLCD *this, bool full);
//void lcd_load(NokiaLCD *this, bool full);
#define lcd_save lcd_noop
#define lcd_load lcd_noop
inline void lcd_noop(NokiaLCD *this, bool full) {}

#include "nokia_font.inc"

// save the screen buffer to flash (set this->page first)
void lcd_save(NokiaLCD *this, bool full)
{
    if (full)
    {
        flashWrite((byte*)&this->s, this->page, 0, sizeof(LCDState));
    }
    else
    {
        flashWrite((byte*)&this->s.flat, this->page, 0, 256);
    }
}

// load the screen buffer from flash (set this->page first)
void lcd_load(NokiaLCD *this, bool full)
{
    if (full)
    {
        flashRead((byte*)&this->s, this->page, 0, sizeof(LCDState));
    }
    else
    {
        flashRead((byte*)&this->s.flat, this->page, 0, 256);
    }
}

// switch to a new window location
void lcd_window(NokiaLCD *this, byte left, byte top, byte width, byte height)
{
    lcd_save(this, false);
    this->s.left = left;
    this->s.top = top;
    this->s.width = width;
    this->s.height = height;
    this->s.x = this->s.y = 0;
    lcd_load(this, false);
}

// turn on and reset the LCD
void lcd_init(NokiaLCD *this, Port port, Pin sce, Pin reset, Pin dc, Pin sdin, Pin sclk, byte left, byte top, byte width, byte height)
{
    this->port = port;
    this->SCE = sce;
    this->RESET = reset;
    this->DC = dc;
    this->SDIN = sdin;
    this->SCLK = sclk;

    // set up comm pins
    pinMode(this->port, this->SCE,   OUTPUT);
    pinMode(this->port, this->RESET, OUTPUT);
    pinMode(this->port, this->DC,    OUTPUT);
    pinMode(this->port, this->SDIN,  OUTPUT);
    pinMode(this->port, this->SCLK,  OUTPUT);

    // reset the screen
    digitalWrite(this->port, this->RESET, LOW);
    sleep(1);
    digitalWrite(this->port, this->RESET, HIGH);

    lcd_write(this,  LCD_CMD, 0x21 ); // LCD Extended Commands.
    lcd_write(this,  LCD_CMD, 0xBf ); // Set LCD Vop (Contrast). //B1
    lcd_write(this,  LCD_CMD, 0x03 ); // Set Temp coefficent. //0x04
    lcd_write(this,  LCD_CMD, 0x13 ); // LCD bias mode 1:48. //0x13
    lcd_write(this,  LCD_CMD, 0x0C ); // LCD in normal mode. 
                               //   0x0d for inverse
    lcd_write(this, LCD_C, 0x20);
    lcd_write(this, LCD_C, 0x0C);

    // flash stuff
    this->page = 31; // page 31 is the only one available (see hacked memory.x)
    this->s.left = left;
    this->s.top = top;
    this->s.width = width;
    this->s.height = height;

    // clear internal state
    memset(this->s.buf, 0, sizeof(byte) * this->s.width * this->s.height);
    lcd_raw_clear(this);
    this->s.x = this->s.y = 0;
    lcd_raw_gotoxy(this, this->s.left, this->s.top);
}

// draw a char onscreen at the current position
void lcd_char(NokiaLCD *this, const char character)
{
    int index;

    lcd_write(this, LCD_D, 0x00);              // blank column
    for (index = 0; index < 5; index++) // print character bitmap
    {
        lcd_write(this, LCD_D, ASCII[character - 0x20][index]);
    }
    lcd_write(this, LCD_D, 0x00);              // blank column
}

// clear the screen
void lcd_clear(NokiaLCD *this)
{
    int index;

    lcd_gotoxy(this, 0, 0);
    for (index = 0; index < this->s.width * this->s.height; index++)
    {                                   // clear every pixel
        lcd_write(this, LCD_D, 0x00);          // the LCD auto-updates
    }                                   //   its X/Y position

    memset(this->s.buf, 0, sizeof(byte) * this->s.width * this->s.height);  // clear out screen buffer
}

void lcd_raw_clear(NokiaLCD *this)
{
    int index;

    lcd_raw_gotoxy(this, 0, 0);
    for (index = 0; index < LCD_X * LCD_Y/8; index++)
    {
        lcd_raw_write(this, LCD_D, 0x00);
    }
}

// draw a string onscreen starting at the current position
void lcd_string(NokiaLCD *this, const char *characters)
{
    while (*characters)
    {
        lcd_char(this, *characters++);
    }
}

// draw an integer onscreen starting at the current position
// this is basically itoa() and lcd_string(this, ) rolled into one
void lcd_itoa(NokiaLCD *this, int val)
{
    char str[20] = "";     // buffer overflow waiting to happen
    int i;

    // negative?
    if (val < 0)
    {
        lcd_char(this, '-'); // show minus sign
        val = -val;        // take absolute value for conversion
    }

    // zero?
    if (val == 0)          // short-circuit for the trivial case
    {
        lcd_char(this, '0');
        return;
    }

    // convert
    for (i = 0; val; ++i)  // here is the itoa()
    {
        str[i] = (val%10) + '0';
        val /= 10;
    }

    // print
    for (--i; i >= 0; --i) // lcd_string(this, ) in reverse
    {
        lcd_char(this, str[i]);
    }
}

void lcd_raw_write(NokiaLCD *this, byte dc, byte data)
{
    digitalWrite(this->port, this->DC, dc);                    // set data/command
    digitalWrite(this->port, this->SCE, LOW);                  // chip select
    shiftOut(this->port, this->SDIN, this->port, this->SCLK, MSBFIRST, data);// software SPI
    digitalWrite(this->port, this->SCE, HIGH);                 // chip deselect
}

// write a byte of data out to the LCD over SPI+1
void lcd_write(NokiaLCD *this, byte dc, byte data)
{
    lcd_raw_write(this, dc, data);

    // update buffer
    if (dc == LCD_D)
    {
        this->s.buf[this->s.x][this->s.y] = data;
     
        // keep track of coordinates
        this->s.x++;
        if (this->s.x > this->s.width - 1)
        {
           this->s.x = 0;
           this->s.y++;
           if (this->s.y > this->s.height - 1)
           {
               this->s.y = 0;
           }

           // we are not relying on the LCD to wrap automatically
           lcd_gotoxy(this, this->s.x, this->s.y);
        }
    }
}

void lcd_raw_gotoxy(NokiaLCD *this, int xx, int yy)
{
    lcd_raw_write(this,  0, 0x80 | xx);  // Column
    lcd_raw_write(this,  0, 0x40 | yy);  // Row
}

// move to a position (xx=0..83, yy=0..5)
void lcd_gotoxy(NokiaLCD *this, int xx, int yy)
{
    lcd_raw_gotoxy(this, xx + this->s.left, yy + this->s.top);

    this->s.x = xx;               // keep track in MSP430 memory, too
    this->s.y = yy;
}

// draw a dot at a specified position (xx=0..83, yy=0..47)
void lcd_dot(NokiaLCD *this, int xx, int yy)
{
    xx = xx % this->s.width;          // cap coordinates at screen edges
    yy = yy % this->s.height;

    lcd_gotoxy(this, xx, yy/8); // move to specified position

    this->s.buf[this->s.x][this->s.y] |= (1 << (yy%8));  
          // XOR the bit into the screen buffer this prevents 
          // clearing out an entire 8-pixel column since the screen
          // can only be addressed bytewise in the Y dimension

    lcd_write(this, LCD_D, this->s.buf[this->s.x][this->s.y]); // write out 8 pixels
}

// draw a frame around the screen
void lcd_drawbox(NokiaLCD *this)
{
    unsigned char j;

    for(j = 0; j < this->s.width; j++)  // top
    {
        lcd_gotoxy(this, j, 0);
        lcd_write(this, 1, 0x01);
    }

    for(j = 0; j < this->s.width; j++)  // bottom
    {
        lcd_gotoxy(this, j, this->s.height - 1);
        lcd_write(this, 1, 0x80);
    }

    for(j = 0; j < this->s.height; j++)   // right
    {
        lcd_gotoxy(this, this->s.width - 1, j);
        lcd_write(this, 1, 0xFF);
    }

    for(j = 0; j < this->s.height; j++)   // left
    {
        lcd_gotoxy(this, 0, j);
        lcd_write(this, 1, 0xFF);
    }
}

