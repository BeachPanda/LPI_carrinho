#include "car.h"
#include "math.h"
#include "joyStick.h"
#include "define.h"
#include "motorControl.h"

//#define DEBUG1
#include "debug.h"

extern points route;

extern char pidFlag;

void carInit(car *c)
{
	c->ornt = &orient;
	c->pos = &mouz;
	c->route = &route;
	c->status = 0;
	
	initMotorControl();
	initSteering();
	startPID();	
	//initHorn();
}

uint16_t updateCarStatus(car *c)
{
	uint16_t stat = 0;
	stat |= IRread(); //obstacle on the sides?
	
	//stat |= USread(); //obstacle front/right?
	//updateAccel();
	if(accel.z < -1100) //upside down?
		stat |= UPSIDEDOWN;
	
	c->status = stat;
	
	angle.y = c->pos->teta;

	return stat;
}

void sendReport(car *c)
{
	DBG1("Px %lld\n"
			 "Py %lld\n"
			 "Or %.1f\n"
			 "Obst %02X\n"
			 "NxtP X %lld Y %lld\n"
			 , c->pos->absX
			 , c->pos->absY
			 , c->pos->teta
			 , c->status
			 , c->route->x[c->route->start]
			 , c->route->y[c->route->start]);
}

void sendPos(car *car)
{
	DBG1("Px %lld\nPy %lld\n\n", car->pos->absX, car->pos->absY);
}

void initHorn()
{	
	
}

void playHorn(void)
{
	
}

//A0
void TIM5_IRQHandler(void)
{	
  if (TIM_GetITStatus(TIM5, TIM_IT_CC1) != RESET){   
    TIM_ClearITPendingBit(TIM5, TIM_IT_CC1); 
	}
}

char CHCKSTAT(uint16_t stat, uint16_t mask)
{
	static uint16_t prevStat = 0;
	char r;
	
	r = (stat ^ prevStat) & (mask & stat); //para detetar transição de livre para obstaculo
	// r = (stat ^ prevStat) & mask; //para detetar todas as transições
	
	prevStat = (stat & mask) ? (prevStat | mask) : (prevStat & (~mask));
	return r;
}
