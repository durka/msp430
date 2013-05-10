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

// ASCII+2 font for the LCD screen
// stored in flash
static const byte ASCII[][5] =
{
    // characters are defined as bitmaps
    // each byte is a column
    // b0    b1    b2    b3    b4      hex char
    {0x00, 0x00, 0x00, 0x00, 0x00} // 20  
    ,{0x00, 0x00, 0x5f, 0x00, 0x00} // 21 !
    ,{0x00, 0x07, 0x00, 0x07, 0x00} // 22 "
    ,{0x14, 0x7f, 0x14, 0x7f, 0x14} // 23 #
    ,{0x24, 0x2a, 0x7f, 0x2a, 0x12} // 24 $
    ,{0x23, 0x13, 0x08, 0x64, 0x62} // 25 %
    ,{0x36, 0x49, 0x55, 0x22, 0x50} // 26 &
    ,{0x00, 0x05, 0x03, 0x00, 0x00} // 27 '
    ,{0x00, 0x1c, 0x22, 0x41, 0x00} // 28 (
    ,{0x00, 0x41, 0x22, 0x1c, 0x00} // 29 )
    ,{0x14, 0x08, 0x3e, 0x08, 0x14} // 2a *
    ,{0x08, 0x08, 0x3e, 0x08, 0x08} // 2b +
    ,{0x00, 0x50, 0x30, 0x00, 0x00} // 2c ,
    ,{0x08, 0x08, 0x08, 0x08, 0x08} // 2d -
    ,{0x00, 0x60, 0x60, 0x00, 0x00} // 2e .
    ,{0x20, 0x10, 0x08, 0x04, 0x02} // 2f /
    ,{0x3e, 0x51, 0x49, 0x45, 0x3e} // 30 0
    ,{0x00, 0x42, 0x7f, 0x40, 0x00} // 31 1
    ,{0x42, 0x61, 0x51, 0x49, 0x46} // 32 2
    ,{0x21, 0x41, 0x45, 0x4b, 0x31} // 33 3
    ,{0x18, 0x14, 0x12, 0x7f, 0x10} // 34 4
    ,{0x27, 0x45, 0x45, 0x45, 0x39} // 35 5
    ,{0x3c, 0x4a, 0x49, 0x49, 0x30} // 36 6
    ,{0x01, 0x71, 0x09, 0x05, 0x03} // 37 7
    ,{0x36, 0x49, 0x49, 0x49, 0x36} // 38 8
    ,{0x06, 0x49, 0x49, 0x29, 0x1e} // 39 9
    ,{0x00, 0x36, 0x36, 0x00, 0x00} // 3a :
    ,{0x00, 0x56, 0x36, 0x00, 0x00} // 3b ;
    ,{0x08, 0x14, 0x22, 0x41, 0x00} // 3c <
    ,{0x14, 0x14, 0x14, 0x14, 0x14} // 3d =
    ,{0x00, 0x41, 0x22, 0x14, 0x08} // 3e >
    ,{0x02, 0x01, 0x51, 0x09, 0x06} // 3f ?
    ,{0x32, 0x49, 0x79, 0x41, 0x3e} // 40 @
    ,{0x7e, 0x11, 0x11, 0x11, 0x7e} // 41 A
    ,{0x7f, 0x49, 0x49, 0x49, 0x36} // 42 B
    ,{0x3e, 0x41, 0x41, 0x41, 0x22} // 43 C
    ,{0x7f, 0x41, 0x41, 0x22, 0x1c} // 44 D
    ,{0x7f, 0x49, 0x49, 0x49, 0x41} // 45 E
    ,{0x7f, 0x09, 0x09, 0x09, 0x01} // 46 F
    ,{0x3e, 0x41, 0x49, 0x49, 0x7a} // 47 G
    ,{0x7f, 0x08, 0x08, 0x08, 0x7f} // 48 H
    ,{0x00, 0x41, 0x7f, 0x41, 0x00} // 49 I
    ,{0x20, 0x40, 0x41, 0x3f, 0x01} // 4a J
    ,{0x7f, 0x08, 0x14, 0x22, 0x41} // 4b K
    ,{0x7f, 0x40, 0x40, 0x40, 0x40} // 4c L
    ,{0x7f, 0x02, 0x0c, 0x02, 0x7f} // 4d M
    ,{0x7f, 0x04, 0x08, 0x10, 0x7f} // 4e N
    ,{0x3e, 0x41, 0x41, 0x41, 0x3e} // 4f O
    ,{0x7f, 0x09, 0x09, 0x09, 0x06} // 50 P
    ,{0x3e, 0x41, 0x51, 0x21, 0x5e} // 51 Q
    ,{0x7f, 0x09, 0x19, 0x29, 0x46} // 52 R
    ,{0x46, 0x49, 0x49, 0x49, 0x31} // 53 S
    ,{0x01, 0x01, 0x7f, 0x01, 0x01} // 54 T
    ,{0x3f, 0x40, 0x40, 0x40, 0x3f} // 55 U
    ,{0x1f, 0x20, 0x40, 0x20, 0x1f} // 56 V
    ,{0x3f, 0x40, 0x38, 0x40, 0x3f} // 57 W
    ,{0x63, 0x14, 0x08, 0x14, 0x63} // 58 X
    ,{0x07, 0x08, 0x70, 0x08, 0x07} // 59 Y
    ,{0x61, 0x51, 0x49, 0x45, 0x43} // 5a Z
    ,{0x00, 0x7f, 0x41, 0x41, 0x00} // 5b [
    ,{0x02, 0x04, 0x08, 0x10, 0x20} // 5c ¥
    ,{0x00, 0x41, 0x41, 0x7f, 0x00} // 5d ]
    ,{0x04, 0x02, 0x01, 0x02, 0x04} // 5e ^
    ,{0x40, 0x40, 0x40, 0x40, 0x40} // 5f _
    ,{0x00, 0x01, 0x02, 0x04, 0x00} // 60 `
    ,{0x20, 0x54, 0x54, 0x54, 0x78} // 61 a
    ,{0x7f, 0x48, 0x44, 0x44, 0x38} // 62 b
    ,{0x38, 0x44, 0x44, 0x44, 0x20} // 63 c
    ,{0x38, 0x44, 0x44, 0x48, 0x7f} // 64 d
    ,{0x38, 0x54, 0x54, 0x54, 0x18} // 65 e
    ,{0x08, 0x7e, 0x09, 0x01, 0x02} // 66 f
    ,{0x0c, 0x52, 0x52, 0x52, 0x3e} // 67 g
    ,{0x7f, 0x08, 0x04, 0x04, 0x78} // 68 h
    ,{0x00, 0x44, 0x7d, 0x40, 0x00} // 69 i
    ,{0x20, 0x40, 0x44, 0x3d, 0x00} // 6a j
    ,{0x7f, 0x10, 0x28, 0x44, 0x00} // 6b k
    ,{0x00, 0x41, 0x7f, 0x40, 0x00} // 6c l
    ,{0x7c, 0x04, 0x18, 0x04, 0x78} // 6d m
    ,{0x7c, 0x08, 0x04, 0x04, 0x78} // 6e n
    ,{0x38, 0x44, 0x44, 0x44, 0x38} // 6f o
    ,{0x7c, 0x14, 0x14, 0x14, 0x08} // 70 p
    ,{0x08, 0x14, 0x14, 0x18, 0x7c} // 71 q
    ,{0x7c, 0x08, 0x04, 0x04, 0x08} // 72 r
    ,{0x48, 0x54, 0x54, 0x54, 0x20} // 73 s
    ,{0x04, 0x3f, 0x44, 0x40, 0x20} // 74 t
    ,{0x3c, 0x40, 0x40, 0x20, 0x7c} // 75 u
    ,{0x1c, 0x20, 0x40, 0x20, 0x1c} // 76 v
    ,{0x3c, 0x40, 0x30, 0x40, 0x3c} // 77 w
    ,{0x44, 0x28, 0x10, 0x28, 0x44} // 78 x
    ,{0x0c, 0x50, 0x50, 0x50, 0x3c} // 79 y
    ,{0x44, 0x64, 0x54, 0x4c, 0x44} // 7a z
    ,{0x00, 0x08, 0x36, 0x41, 0x00} // 7b {
    ,{0x00, 0x00, 0x7f, 0x00, 0x00} // 7c |
    ,{0x00, 0x41, 0x36, 0x08, 0x00} // 7d }
    ,{0x10, 0x08, 0x08, 0x10, 0x08} // 7e left arrow
    ,{0x00, 0x06, 0x09, 0x09, 0x06} // 7f right arrow

    // (these are not really part of ASCII)
    ,{0x3e, 0x49, 0x49, 0x49, 0x3e} // 80 theta
    ,{0x00, 0x00, 0x07, 0x05, 0x07} // 81 degrees
};

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

    for (index = 0; index < LCD_X * LCD_Y / 8; index++)
    {                                   // clear every pixel
        lcd_write(this, LCD_D, 0x00);          // the LCD auto-updates
    }                                   //   its X/Y position

    //memset(this->buf, 0, sizeof(byte)*LCD_X*LCD_Y/8);  // clear out screen buffer
}

// turn on and reset the LCD
void lcd_init(NokiaLCD *this, Port port, Pin sce, Pin reset, Pin dc, Pin sdin, Pin sclk)
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
    lcd_write(this,  LCD_CMD, 0x04 ); // Set Temp coefficent. //0x04
    lcd_write(this,  LCD_CMD, 0x12 ); // LCD bias mode 1:48. //0x13
    lcd_write(this,  LCD_CMD, 0x0C ); // LCD in normal mode. 
                               //   0x0d for inverse
    lcd_write(this, LCD_C, 0x20);
    lcd_write(this, LCD_C, 0x0C);

    // clear internal state
    memset(this->buf, 0, sizeof(byte)*LCD_X*LCD_Y/8);
    this->lcd_x = this->lcd_y = 0;
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

// write a byte of data out to the LCD over SPI+1
void lcd_write(NokiaLCD *this, byte dc, byte data)
{
    // update buffer
    if (dc == LCD_D)
    {
        this->buf[this->lcd_x][this->lcd_y] = data;
     
        // keep track of coordinates
        this->lcd_x++;
        if (this->lcd_x > LCD_X - 1)     // the LCD wraps around its internal
        {                         // coordinates automatically
           this->lcd_x = 0;
           this->lcd_y++;
        }
    }

    digitalWrite(this->port, this->DC, dc);                    // set data/command
    digitalWrite(this->port, this->SCE, LOW);                  // chip select
    shiftOut(this->port, this->SDIN, this->port, this->SCLK, MSBFIRST, data);// software SPI
    digitalWrite(this->port, this->SCE, HIGH);                 // chip deselect
}

// move to a position (xx=0..83, yy=0..5)
void lcd_gotoxy(NokiaLCD *this, int xx, int yy)
{
    lcd_write(this,  0, 0x80 | xx);  // Column
    lcd_write(this,  0, 0x40 | yy);  // Row

    this->lcd_x = xx;               // keep track in MSP430 memory, too
    this->lcd_y = yy;
}

// draw a dot at a specified position (xx=0..83, yy=0..47)
void lcd_dot(NokiaLCD *this, int xx, int yy)
{
    xx = xx % LCD_X;          // cap coordinatesm at screen edges
    yy = yy % LCD_Y;

    lcd_gotoxy(this, xx, yy/8); // move to specified position

    this->buf[this->lcd_x][this->lcd_y] |= (1 << (yy%8));  
          // XOR the bit into the screen buffer this prevents 
          // clearing out an entire 8-pixel column since the screen
          // can only be addressed bytewise in the Y dimension

    lcd_write(this, LCD_D, this->buf[this->lcd_x][this->lcd_y]); // write out 8 pixels
}

// draw a frame around the screen
void lcd_drawbox(NokiaLCD *this)
{
    unsigned char j;

    for(j = 0; j < LCD_X; j++)  // top
    {
        lcd_gotoxy(this, j, 0);
        lcd_write(this, 1, 0x01);
    }

    for(j = 0; j < LCD_X; j++)  // bottom
    {
        lcd_gotoxy(this, j, LCD_Y/8 - 1);
        lcd_write(this, 1, 0x80);
    }

    for(j = 0; j < LCD_Y/8; j++)   // right
    {
        lcd_gotoxy(this, LCD_X - 1, j);
        lcd_write(this, 1, 0xFF);
    }

    for(j = 0; j < LCD_Y/8; j++)   // left
    {
        lcd_gotoxy(this, 0, j);
        lcd_write(this, 1, 0xFF);
    }
}

