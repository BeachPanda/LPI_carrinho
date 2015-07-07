#define _IR_SENSORS_C
#include "IRSensors.h"

void IRinit()
{
	GPIO_InitTypeDef GPIO_InitDef; //Where GPIO_InitDef is variable to work with struct
	GPIO_InitDef.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5; //0x3f; //6bits
		
	//Enable clock for GPOIB
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

	GPIO_InitDef.GPIO_Mode = GPIO_Mode_IN;

	GPIO_InitDef.GPIO_OType = GPIO_OType_PP;

	GPIO_InitDef.GPIO_PuPd = GPIO_PuPd_UP;
	//50MHz pin speed
	GPIO_InitDef.GPIO_Speed = GPIO_Speed_50MHz;

	//Initialize pins on GPIOB port
	GPIO_Init(GPIOB, &GPIO_InitDef);
}
