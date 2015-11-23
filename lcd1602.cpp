/* LCD1602 AVR library
 *
 * Copyright (C) 2015 Sergey Denisov.
 * Rewritten by Sergey Denisov aka LittleBuster (DenisovS21@gmail.com)
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public Licence
 * as published by the Free Software Foundation; either version 3
 * of the Licence, or (at your option) any later version.
 */

#include "lcd1602.h"


void LCD1602::putnibble(char t)
{
	t <<= 4;
	i2c.sendPocket (Led_port |= 0x04, SLA_W);
	_delay_us(50);
	i2c.sendPocket(Led_port|t, SLA_W);
	i2c.sendPocket (Led_port &= ~0x04, SLA_W);
	_delay_us(50);
}

void LCD1602::sendByte(char c, char rs)
{
    char highc = 0;
    highc = c >> 4;
	if (rs == LCD_COMMAND)
		i2c.sendPocket(Led_port &=~ 0x01, SLA_W);
	else
		i2c.sendPocket(Led_port |= 0x01, SLA_W);
    putnibble(highc);
    putnibble(c);
}

void LCD1602::sendChar(char c)
{
	char highc = 0;

	highc = c >> 4;
	i2c.sendPocket (Led_port |= 0x01, SLA_W);
	putnibble(highc);
	putnibble(c);
}

void LCD1602::init()
{
	Led_port = 0;
	i2c.init();
    _delay_ms(15);
    putnibble(0b00000011);
    _delay_ms(4);
    putnibble(0b00000011);
    _delay_us(100);
    putnibble(0b00000011);
	 _delay_ms(1);
    putnibble(0b00000010);
	_delay_ms(1);
    sendByte(0x28, LCD_COMMAND);
    _delay_ms(1);
    sendByte(0x0C, LCD_COMMAND);
    _delay_ms(1);
    sendByte(0x06, LCD_COMMAND);
    _delay_ms(1);
	i2c.sendPocket (Led_port |= 0x08, SLA_W);
	i2c.sendPocket (Led_port &=~ 0x02, SLA_W);
}

void LCD1602::clear()
{
    sendByte(0x01, LCD_COMMAND);
    _delay_us(1500);
}

void LCD1602::goToXY(char col, char row)
{
  char adr;

  adr = 0x40*row + col;
  adr |= 0x80;
  sendByte(adr, LCD_COMMAND);
}

void LCD1602::sendString (const char str[])
{
	uint8_t i;

	for(i = 0; str[i] != '\0'; i++)
		sendChar(str[i]);
}
