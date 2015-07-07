#ifndef _CAR_H
#define _CAR_H

#define ACCEL_DEBUG

#include "IRSensors.h"
#include "Accelerometer.h"
#include "steering.h"
#include "mouse.h"
#include "stdint.h"

/*
#define IR_FRONTR 0x0001
#define IR_FRONTL 0x0002
#define IR_MIDDLR 0x0004
#define IR_MIDDLL 0x0008
#define IR_BACKR  0x0010
#define IR_BACKL	0x0020

#define UPSIDEDOWN 0x0040
#define RIGHTSIDE  0x0080
#define LEFTSIDE   0x0100
*/

#define ALL_SENSORS 0x01FF

#define RADIUS ((float)14)
#define PERIMETER ((float)(2 * 3.141592 * RADIUS))
#define CONV_TO_D(x) (x * (float)0.002)
#define CONV_TO_UNIT(x) (x * (float)500)

struct car
{
	orientation *ornt;
	mouse *pos;
	points *route;
	float vel;
	uint16_t status;
	
	int pwmLeft;
	int pwmRight;
};

typedef struct car car;

uint16_t updateCarStatus(car *c);

char CHCKSTAT(uint16_t a, uint16_t b);

void carInit(car *c);

void initHorn(void);

void playHorn(void);

void sendPos(car *c);

void sendReport(car *c);

#endif
