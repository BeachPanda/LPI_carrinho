#include "stm32f4xx_gpio.h"

#define GREEN_ON() GPIO_SetBits(GPIOD, GPIO_Pin_12)
#define GREEN_OFF() GPIO_ResetBits(GPIOD, GPIO_Pin_12)

#define ORANG_ON() GPIO_SetBits(GPIOD, GPIO_Pin_13)
#define ORANG_OFF() GPIO_ResetBits(GPIOD, GPIO_Pin_13)

#define RED_ON() GPIO_SetBits(GPIOD, GPIO_Pin_14)
#define RED_OFF() GPIO_ResetBits(GPIOD, GPIO_Pin_14)

#define BLUE_ON() GPIO_SetBits(GPIOD, GPIO_Pin_15)
#define BLUE_OFF() GPIO_ResetBits(GPIOD, GPIO_Pin_15)

void initLeds(void);
