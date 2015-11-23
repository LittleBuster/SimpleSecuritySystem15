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

#ifndef __LCD1602_H__
#define __LCD1602_H__


#include <avr/io.h>
#include <util/delay.h>

#include "i2c.h"


#define LCD_ADDR 0b00100111
#define SLA_W LCD_ADDR<<1
#define LCD_COMMAND 0
#define LCD_DATA 1


class LCD1602
{
private:
	I2C i2c;
	unsigned char Led_port;

	void putnibble(char t);

public:
	void sendByte(char c, char rs);

	void sendChar(char c);

	void init();

	void clear();

	void goToXY(char col, char row);

	void sendString (const char str[]);
};


#endif
