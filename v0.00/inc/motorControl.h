#include "stm32f4xx.h"                  // Device header
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_tim.h"
#include "stm32f4xx_rcc.h"
#include "misc.h"

struct orientation
{
	float dir;
	float vel;
	float maxVal;
};

typedef struct orientation orientation;

extern orientation orient;

int setOrientation(float dir, float vel);

void InitializeTimer(int period);

void InitializePWMChannel(void);

void initMotorControl(void);
