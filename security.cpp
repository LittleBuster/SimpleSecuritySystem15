#include "security.h"
#include <avr/io.h>
#include <stdio.h>
#include <string.h>


unsigned char my_key[] = {0x01, 0x7E, 0x72, 0x41, 0x0E, 0x00, 0x00, 0xC3};


/*
 * Private members
 */
void Security::startAlarm(void)
{
	/* On alarm relay */
	PORTB &= ~(1 << PB2);

	for (;;) {
		_delay_ms(500);
		/* Buzzer on */
		DDRC |= (1 << 0);
		PORTC |= (1 << 0);

		/* On LEDs */
		PORTD |= (1 << PD6);
		PORTD |= (1 << PD7);

		_delay_ms(500);
		/* Off LEDs */
		PORTD &= ~(1 << PD6);
		PORTD &= ~(1 << PD7);

		/* Buzzer off */
		PORTC &= ~(1 << 0);
		DDRC &= ~(1 << 0);
	}
}

void Security::checkTablet(void)
{
	wire.findROM(&diff, &id[0]);
	if ((diff != OW_PRESENCE_ERR) && (diff != OW_DATA_ERR)) {
		unsigned char *my_ptr = my_key;
		unsigned char *id_ptr = id;
		bool f = true;

		/* Check codes */
		for (char i = 0; i < OW_ROMCODE_SIZE; i++) {
			if (*my_ptr++ != *id_ptr++) {
				f = false;
				break;
			}
		}
		if (f == true) {
			f = false;
			if (isOn == true) {
				isOn = false;
				PORTD |= (1 << PD7); //Red LED on
				PORTD &= ~(1 << PD6); //Green LED off
			}
			else {
				isOn = true;
				PORTD &= ~(1 << PD7); //Red LED off
				PORTD |= (1 << PD6); //Green LED on
			}

			/* long signal OK */
			DDRC |= (1 << 0);
			PORTC |= (1 << 0);
			_delay_ms(3000);
			DDRC &= ~(1 << 0);
			PORTC &= ~(1 << 0);
		}
	}
}

/*
 * Public members
 */
void Security::init(void)
{
	isOn = true;
	isAlarm = false;
	sensors.isDoor = false;
	sensors.isVer = false;
	sensors.isRoom = false;
	sensors.isKitchen = false;
	motions.kmotion = 0;
	motions.rmotion = 0;
	motions.vmotion = 0;

	wire.setPin(5);

	/* Init sensors ports */
	DDRD &= ~(1 << PD3);
	DDRD &= ~(1 << PD4);
	DDRB &= ~(1 << PB0);
	DDRB &= ~(1 << PB1);

	/* Control panel */
	DDRD &= ~(1 << PD5); //Tablet
	DDRD |= (1 << PD7);
	PORTD |= (1 << PD7); //Red LED on
	DDRD |= (1 << PD6);
	PORTD &= ~(1 << PD6); //Green LED off

	/* Alarm relay off */
	DDRB |= (1 << PB2);
	PORTB |= (1 << PB2);
}

bool Security::loop(void)
{
	checkTablet();

	if (isOn == true) {
		if (isAlarm == true)
			startAlarm();

		/* Check veranda motion */
		if ((PIND & (1 << PD3)) != 0) {
			sensors.isVer = true;
			_delay_ms(3000);
			motions.vmotion++;

			if (motions.vmotion == 3)
				isAlarm = true;
		} else
			sensors.isVer = false;

		/* Check Door gerkon */
		if ((PIND & (1 << PD4)) == 0) {
			sensors.isDoor = true;
			isAlarm = true;
		} else
			sensors.isDoor = false;

		/* Check kitchen motion */
		if ((PINB & (1 << PB0)) != 0) {
			sensors.isKitchen = true;
			_delay_ms(3000);
			motions.kmotion++;

			if (motions.kmotion == 3)
				isAlarm = true;
		} else
			sensors.isKitchen = false;

		/* Check room motion */
		if ((PINB & (1 << PB1)) != 0) {
			sensors.isRoom = true;
			_delay_ms(3000);
			motions.rmotion++;

			if (motions.rmotion == 3)
				isAlarm = true;
			 else
				sensors.isRoom = false;
		}
		return true;
	} else
		return false;
}
