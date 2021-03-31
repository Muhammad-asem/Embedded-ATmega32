/*
 * LCD.h
 *
 * Created: 1/29/2016 4:49:12 PM
 *  Author: Muhammad Asem
 */ 
//#define LCD_WIDTH 16
#define LCD_LENGTH 16u
#ifndef LCD_H
#define LCD_H

void lcd_init(void);

void lcd_writeChar(unsigned char);
void lcd_writeString( char*);
void lcd_clear(void);
void LCD_GoToXy(char PosX,char PosY);

#endif 