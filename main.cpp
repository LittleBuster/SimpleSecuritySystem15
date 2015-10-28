#include <stdio.h>
#include <string.h>
#include <avr/interrupt.h>

#include "security.h"
#include "lcd1602.h"


LCD1602 lcd;
Security security;


void showData(void)
{
	char txt[17];
	SSensors ssensors;

	ssensors = security.getSensorsState();
	lcd.goToXY(0, 0);
	if (ssensors.isVer == true)
		strcpy(txt, "V:O");
	else
		strcpy(txt, "V:X");

	if (ssensors.isDoor == true)
		strcat(txt, " D:O");
	else
		strcat(txt, " D:X");

	if (ssensors.isKitchen == true)
		strcat(txt, " K:O");
	else
		strcat(txt, " K:X");

	if (ssensors.isRoom == true)
		strcat(txt, " R:O");
	else
		strcat(txt, " R:X");
	lcd.sendString(txt);

	if (security.getAlarmState() == true) {
		lcd.goToXY(0, 1);
		strcpy(txt, "     Alarm!!!");
		lcd.sendString(txt);
	} else {
		strcpy(txt, "     SSS15");
		lcd.sendString(txt);
	}
}

void showOff(void)
{
	char txt[16];

	strcpy(txt, "Security is off");
	lcd.goToXY(0, 0);
	lcd.sendString(txt);
	_delay_ms(500);
	lcd.clear();
}

int main()
{
	security.init();
	lcd.init();
	lcd.clear();

	lcd.sendString("Simple Security");
	lcd.goToXY(0, 1);
	lcd.sendString("  System 2015");
	_delay_ms(3000);
	lcd.clear();

	for (;;) {
		if (security.loop())
			showData();
		else
			showOff();
	}
}

//А у меня есть мясорубка 8 скоростей
//Перемолола вчера мясо и мешок костей
