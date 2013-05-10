/*
* Library for Nokia 5110 LCD with PCD8544 controller
*
* Ported from Arduino sample code and enhanced
*
* Alex Burka
* Katherine Bertaut
* November 2010
*/


/* TODO
 * some more flexible screen buffering method
 * currently: buffer only left half of the screen, ignore bugs
 * idea: "small" window buffer in RAM, whole buffer in flash
 *       graphics allowed only in one window at a time
 *       in theory, could even do window layering
 */

#ifndef _BURKA_NOKIA_LCD_H_
#define _BURKA_NOKIA_LCD_H_

#include "common.h"

#define LCD_X     42 // screen width (really it's 84, see TODO above)
#define LCD_Y     48 // screen height

typedef struct
{
    Port port;
    Pin SCE;    // LCD CS  .... Pin 3
    Pin RESET;  // LCD RST .... Pin 1
    Pin DC;     // LCD Dat/Com. Pin 5
    Pin SDIN;   // LCD SPIDat . Pin 6
    Pin SCLK;   // LCD SPIClk . Pin 4
                // LCD Vcc .... Pin 8
                // LCD Gnd .... Pin 2
                // LCD Vlcd ... Pin 7 
                //    (connected to Vcc)

    // internal LCD state
    int lcd_x, lcd_y;   // current position
    byte buf[LCD_X][LCD_Y/8];    // in-memory screen buffer

} NokiaLCD;

// LCD functions
void lcd_char(NokiaLCD *this, const char character);
                         // draw a char onscreen at the
                         //   current position
void lcd_clear(NokiaLCD *this);     // clear the screen
void lcd_init(NokiaLCD *this, Port port, Pin sce, Pin reset, Pin dc, Pin sdin, Pin sclk); // turn on and reset the LCD
void lcd_string(NokiaLCD *this, const char *characters);
                         // draw a string onscreen starting
                         //   at the current position
void lcd_itoa(NokiaLCD *this, int val);   // draw an integer onscreen starting
                         //   at the current position
void lcd_write(NokiaLCD *this, byte dc, byte data);
                         // write a byte of data out to the
                         //   LCD over SPI+1
void lcd_gotoxy(NokiaLCD *this, int xx, int yy);
                         // move to a position (xx=0..83, yy=0..5)
void lcd_drawbox(NokiaLCD *this);   // draw a frame around the screen
void lcd_dot(NokiaLCD *this, int xx, int yy);
                         // draw a dot at a specified position
                         //   (xx=0..83, yy=0..47)


#endif /*NOKIA_LCD_H_*/

