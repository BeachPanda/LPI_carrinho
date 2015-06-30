#include "motorControl.h"

TIM_OCInitTypeDef outputChannelInit;
TIM_TimeBaseInitTypeDef timerInitStructure;

orientation orient;

void initMotorControl(void)
{
	InitializeTimer(500);
	InitializePWMChannel();
}

int timPulse(int dutyCycle)
{
	return ((timerInitStructure.TIM_Period + 1) * dutyCycle / 100);
}

void setPWMLeft(float dc)
{
	outputChannelInit.TIM_Pulse = timPulse(dc);
	TIM_OC1Init(TIM4, &outputChannelInit);
	TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);
}

void setPWMRight(float dc)
{
	outputChannelInit.TIM_Pulse = timPulse(dc);
	TIM_OC3Init(TIM4, &outputChannelInit);
	TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);
}

//Set orientation
//dir is [0, 5]
int setOrientation(float dir, float vel)
{
	if((dir > 1) || (dir < -1))
		return 0;
	
	if((vel > 1) || (vel < -1))
		return 0;
	
	orient.dir = dir;
	orient.vel = vel;
	
	if(vel >= 0){
		outputChannelInit.TIM_Pulse = timPulse(vel * 100);
		TIM_OC2Init(TIM4, &outputChannelInit);
		TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);
		
		outputChannelInit.TIM_Pulse = timPulse(0);
		TIM_OC4Init(TIM4, &outputChannelInit);
		TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);
	}
	else if(vel < 0){
		vel = -vel;
		outputChannelInit.TIM_Pulse = timPulse(vel * 100);
		TIM_OC4Init(TIM4, &outputChannelInit);
		TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);
		
		outputChannelInit.TIM_Pulse = timPulse(0);
		TIM_OC2Init(TIM4, &outputChannelInit);
		TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);
	}
	
	//turn right
	if(dir >= 0){
		setPWMLeft(100 * vel);
		//slow down right motor by dir amount
		setPWMRight(100 * vel * (1 - dir));
	}
	//turn left
	else if(dir < 0){
		dir = -dir;
		setPWMRight(100 * vel);
		//slow down left motor by dir amount
		setPWMLeft(100 * vel * (1 - dir));
	}
	
	return 1;
}

void InitializeTimer(int period)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	timerInitStructure.TIM_Prescaler = 0;
	timerInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	timerInitStructure.TIM_Period = (26750000/1000)-1;
	timerInitStructure.TIM_ClockDivision = 0;
	timerInitStructure.TIM_RepetitionCounter = 0;

	TIM_TimeBaseInit(TIM4, &timerInitStructure);
	TIM_Cmd(TIM4, ENABLE);
}

void InitializeLEDs(void)
{		
	GPIO_InitTypeDef gpioStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

	GPIO_PinAFConfig(GPIOD, GPIO_PinSource12, GPIO_AF_TIM4);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource13, GPIO_AF_TIM4);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource14, GPIO_AF_TIM4);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource15, GPIO_AF_TIM4);
	
	gpioStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	gpioStructure.GPIO_Mode = GPIO_Mode_AF;
	gpioStructure.GPIO_Speed = GPIO_Speed_50MHz;
	gpioStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOD, &gpioStructure);
}

void InitializePWMChannel()
{    
	outputChannelInit.TIM_OCMode = TIM_OCMode_PWM1;

	InitializeLEDs();

	outputChannelInit.TIM_OutputState = TIM_OutputState_Enable;
	outputChannelInit.TIM_OCPolarity = TIM_OCPolarity_High;
}
