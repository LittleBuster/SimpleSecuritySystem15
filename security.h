#ifndef SECURITY_H_
#define SECURITY_H_


#include "OneWire.h"


typedef struct {
	bool isDoor;
	bool isVer;
	bool isRoom;
	bool isKitchen;
} SSensors;

typedef struct {
	uint8_t vmotion;
	uint8_t kmotion;
	uint8_t rmotion;
} Motions;


class Security
{
private:
	OneWire wire;
	Motions motions;
	SSensors sensors;

	bool isOn;
	bool isAlarm;

	unsigned char id[OW_ROMCODE_SIZE];
	unsigned char diff;

	void checkTablet(void);

	void startAlarm(void);

public:
	inline SSensors getSensorsState(void)
	{
		return sensors;
	}

	inline bool getAlarmState(void)
	{
		return isAlarm;
	}

	void init(void);

	bool loop(void);
};


#endif
