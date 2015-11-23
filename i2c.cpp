/* I2C (TwoWire) AVR library
 *
 * Copyright (C) 2015 Sergey Denisov.
 * Rewritten by Sergey Denisov aka LittleBuster (DenisovS21@gmail.com)
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public Licence
 * as published by the Free Software Foundation; either version 3
 * of the Licence, or (at your option) any later version.
 */

#include "i2c.h"

/*
 * Private members
 */
void I2C::startCondition(void)
{
	TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
	while (!(TWCR & (1<<TWINT)));
}

void I2C::stopCondition(void)
{
	TWCR = (1<<TWINT)|(1<<TWSTO)|(1<<TWEN);
}

/*
 * Public members
 */
void I2C::sendByte(unsigned char c)
{
	TWDR = c;
	TWCR = (1<<TWINT)|(1<<TWEN);
	while (!(TWCR & (1<<TWINT)));
}

void I2C::init (void)
{
	TWBR=0xFF;
}

void I2C::sendPocket (unsigned char value,unsigned char adres_rw)
{
	startCondition();
	sendByte(adres_rw);
	sendByte(value);
	stopCondition();
}

unsigned char I2C::recvByte(void)
{
	unsigned char dat;

	TWCR |= (1<<TWINT)|(1<<TWEN)|(1<<TWEA);
	while (!(TWCR & (1<<TWINT)));
	dat = TWDR;

	return dat;
}

unsigned char I2C::recvLastByte(void)
{
	unsigned char dat;

	TWCR |= (1<<TWINT)|(1<<TWEN)|(0<<TWEA);
	while (!(TWCR & (1<<TWINT)));
	dat = TWDR;

	return dat;
}
