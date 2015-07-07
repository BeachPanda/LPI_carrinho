#ifndef _ORIENT_H
#define _ORIENT_H

#include "stm32f4xx.h"                  // Device header
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_tim.h"
#include "stm32f4xx_rcc.h"
#include "misc.h"

struct orientation
{
	float left;
	float right;
};

typedef struct orientation orientation;

extern orientation orient;

int setOrientation(float dir, float vel, int *left, int *right);

void InitializeTimer(int period);

void InitializePWMChannel(void);

void initMotorControl(void);

void goForward(int vel);

void turn(int howMuch);

void setPWMLeftB(float dc);

void setPWMRightB(float dc);

void setPWMLeftF(float dc);

void setPWMRightF(float dc);
#endif
