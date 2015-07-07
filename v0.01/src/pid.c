#include "pid.h"
#include "debug.h"

char pidFlag;

float pid_algor(PID *pid)
{
	float e;

	e = pid->ref - pid->y;

	//em modo manual não é preciso calcular saída
	if(pid->mod == autom)
	{
		//componente proporcional
		pid->u.value = pid->kp.value * e;
		//componente integral
		pid->u.value += pid->ki.kh * (pid->e.sum + e);

		//componente derivativa com filtro
		if(pid->filter.N != 0)
		{
			pid->u.value += pid->kd.kh * (e - pid->e.ant) + pid->filter.kf * pid->u.ant;
		}
		//componente derivativa sem filtro
		else
			pid->u.value += pid->kd.kh * (e - pid->e.ant);

		//valor a que a variável a controlar é = á referência
		pid->u.value += pid->u.o0;

		//se saturou, quer por excesso ou defeito
		if(pid->u.value >= pid->u.max)
			pid->u.value = (float)pid->u.max;
		else if(pid->u.value <= (float)pid->u.min)
			pid->u.value = pid->u.min;
		else
			pid->e.sum = pid->e.sum + e;
	}

	pid->u.ant = pid->u.value;
	pid->e.ant = e;
	
	return pid->u.value/pid->u.max;
}


void PID_configTimer()
{
	TIM_TimeBaseInitTypeDef timerInitStructure;
	NVIC_InitTypeDef nvicStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

	timerInitStructure.TIM_Prescaler = 10000;
	timerInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	timerInitStructure.TIM_Period = 500;
	timerInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	timerInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM3, &timerInitStructure);
	
	nvicStructure.NVIC_IRQChannel = TIM3_IRQn;
	nvicStructure.NVIC_IRQChannelPreemptionPriority = 0;
	nvicStructure.NVIC_IRQChannelSubPriority = 1;
	nvicStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&nvicStructure);
	
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
	TIM_Cmd(TIM3, ENABLE);
}

void commitParam(PID *pid)
{
	float tf, h;

	if(pid->filter.N != 0)											{
		h = 1 / pid->freq													;
		tf = pid->kd.value / pid->filter.N				;
		pid->kd.kh = pid->kd.value/(tf + h)				;
		pid->filter.kf = tf / (tf + h)						;}																				
	else
		pid->kd.kh = pid->kd.value * pid->freq		;

	pid->ki.kh = pid->ki.value / pid->freq;
}

void initPID(PID *pid)
{
	pidFlag = 0;
	pid->kd.max    = 0x7fff;
	pid->kd.value  = 0;
	pid->kd.min    = -0x7fff;
	pid->ki.max    = 0x7fff;
	pid->ki.value  = 0;
	pid->ki.min    = -0x7fff;
	pid->kp.max    = 0x7fff;
	pid->kp.value  = 0;
	pid->kp.min    = -0x7fff;
	pid->u.max     = 0x7fff;
	pid->u.value   = 0;
	pid->u.min     = -0x7fff;
	pid->u.o0      = 0;
	pid->e.ant     = 0;
	pid->e.sum     = 0;
	pid->filter.N  = 0;
	pid->filter.kf = 0;
	pid->ref = 0;
	pid->mod = autom;
	pid->freq = 0;
}

void printPIDSettings(PID pid)
{
	DBG("Ki :%f\n"
			"Kp :%f\n"
			"Kd :%f\n"
			"U :%f\n"
			"N :%d\n"
			"Freq :%f\n"
			"Modo :%d\n",
			pid.ki.value,
			pid.kp.value,
			pid.kd.value,
			pid.u.value,
			pid.filter.N,
			(double)pid.freq,
			(int)pid.mod);
}
