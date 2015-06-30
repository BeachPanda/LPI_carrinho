#include "stm32f4xx.h"
#include "stm32f4xx_adc.h"
#include "stm32f4xx_tim.h"
//#include "stm32f4xx_dma.h"
#include "stm32f4xx_gpio.h"

#define IR_FRONTR 0x0001
#define IR_FRONTL 0x0002
#define IR_MIDDLR 0x0004
#define IR_MIDDLL 0x0008
#define IR_BACKR  0x0010
#define IR_BACKL	0x0020
#define IR_ALL    0x003f

#define IRread() ((~GPIOB->IDR) & IR_ALL)

void IRinit(void);
