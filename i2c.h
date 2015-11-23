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

#ifndef __I2C__
#define __I2C__

#include <avr/io.h>


class I2C
{
private:
	void startCondition(void);

	void stopCondition(void);

public:
	void sendByte(unsigned char c);

	void init (void);

	void sendPocket (unsigned char value,unsigned char adres_rw);

	unsigned char recvByte(void);

	unsigned char recvLastByte(void);
};


#endif
