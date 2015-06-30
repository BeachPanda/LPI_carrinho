#include "car.h"

extern points route;

TIM_OCInitTypeDef timerOCInitStruct;

uint16_t updateCarStatus(car *c)
{
	uint16_t stat = 0;
	stat |= IRread(); //obstacle on the sides?
	
	//stat |= USread(); //obstacle front/right?
	
	updateAccel();
	
	if(accel.z < -1100)//upside down?
		stat |= UPSIDEDOWN;
	
	c->status = stat;
	
	xcoor.y = c->pos->absX;
	ycoor.y = c->pos->absY;
	
	return stat;
}

void sendReport(car *c)
{
	printf("Px %lld\n"
				 "Py %lld\n"
				 "Dir %1.3f\n"
				 "Vel %1.3f\n"
				 "Obst %02X\n"
				 "NxtP X %lld Y %lld\n"
				 , c->pos->absX
				 , c->pos->absY
				 , c->ornt->dir
				 , c->ornt->vel
				 , c->status
				 , c->route->x[c->route->start]
				 , c->route->y[c->route->start]);
}

void sendPos(car *car)
{
	printf("Px %lld\nPy %lld\n\n", car->pos->absX, car->pos->absY);
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


void carInit(car *c)
{
	c->ornt = &orient;
	c->pos = &mouz;
	c->route = &route;
	
	c->status = 0;

	initSteering();
	
	xcoor.kp.value = 10;
	ycoor.kp.value = 10;
	
	IRinit();
	accelInit();
	initMotorControl();
	
	initHorn();
	startPID();
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
