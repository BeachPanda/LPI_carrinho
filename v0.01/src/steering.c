#include "steering.h"
#include "car.h"

#include "define.h"
#ifdef PID
	#define DEBUG1
#endif
#include "debug.h"

extern char pidFlag;

PID dist;
PID angle;

points route;
extern car mrCar;

void TIM3_IRQHandler()
{
	float cang;
	float v;
	static long int yant;
	static int astart;
	
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
	{
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);	
		
		//updateCarStatus(&mrCar);		
		getAngleRef();
	
		cang = pid_algor(&angle);
		
		//dead zone
		if((cang < (float)0.1) && (cang > (float)-0.1)){
			getDistRef();
			v = pid_algor(&dist);	
			
			goForward(-v * 100);
			
		} else
			turn(cang * 100);
		
		if(yant != dist.y){
			DBG("Dist %lld", dist.y);
			yant = dist.y;
			if(dist.y < 20){					
				if(route.start != route.end){	
					route.start++;
				}
			}
		}
		
		if(astart != route.start){
			astart = route.start;
			DBG("Changing target");
			DBG("RPx %lld - RPy %lld", route.x[astart], route.y[astart]);
		}
		
		pidFlag = 1;
	}
}

void loadRoute(void)
{
	int i = 0;

	for(i = 0; i < N_POINTS - 1; i++){
		if(i % 2){
			addPoint(0, -1000);
		}else {
			addPoint(0, 1000);
		}
	}
}

extern void Delay(long int);

void printRoute(void)
{
	int i = 0;
	
	DBG("Printing Route:");
	for(i = 0; i < N_POINTS; i++){
		DBG1("RPx %lld n- RPy %lld", route.x[i], route.y[i]);
		Delay(1);
	}
	DBG1("Start %d", route.start);
	DBG1("End %d", route.end);
}

void initSteering(void)
{
	initPID(&dist);
	initPID(&angle);
	route.start = 0;
	route.end = 0;

	loadRoute();
	//printRoute();
	
	angle.kp.value = 15;
	angle.u.max = 300;
	angle.u.min = -300;
	
	dist.kp.value = 5;
	dist.u.max = 100;
	dist.u.min = -100;
	
	dist.ref = 0;	
}

void addPoint(int64_t x, int64_t y)
{	
	route.x[route.end % N_POINTS] = x;
	route.y[route.end % N_POINTS] = y;	
	route.end++;
}

void switchCurrentPoint(int64_t x, int64_t y)
{
	int i = route.start;
	
	route.x[i] = x;
	route.y[i] = y;
}

float correctAngle(int64_t x, int64_t y)
{
	float a;
	
	a = 180 - atanf((float)y/x) * 180 / (float)3.141592;
	
	a = (a < 0) ? -a : a;
	
	if((x < 0) && (y > 0))
		a = 180 - a;
	
	else if((x < 0) && (y < 0))
		a = 180 + a;
	
	else if((x > 0) && (y < 0))
		a = 360 - a;
	
	else if((x == 0) && (y == 0))
		a = 0;
	
//	DBG1("Px %2lld", mouz.absX);
//	DBG1("Py %2lld", mouz.absY);
	
	return a;
}

void getAngleRef(void)
{
	int64_t dx;
	int64_t dy;
	float da;
	static float ada = 400;
	static float ateta = 400;

	dx = route.x[route.start % N_POINTS] - mrCar.pos->absX;
	dy = route.y[route.start % N_POINTS] - mrCar.pos->absY;
	
	da = correctAngle(dx, dy);
	
	angle.ref = da;
	angle.y = mrCar.pos->teta;
	
	if(ada == 400){
		//DBG("OR %1.2f", ada);
	}
	
	if(ada != da){
		DBG1("OR %1.2f\n", ada);
		ada = da;
	}
	
	if(ateta != mrCar.pos->teta){
		ateta = mrCar.pos->teta;
		DBG1("Or %1.2f", ateta);
	}
	
	//DBG1("OR %1.2f", angle.ref);
	//DBG1("Or %1.2f", mrCar.pos->teta);
	//DBG1("Err %f", angle.e.ant);
}

void getDistRef(void)
{
	int64_t x;
	int64_t y;
	
	x = route.x[route.start % N_POINTS] - mrCar.pos->absX;
	y = route.y[route.start % N_POINTS] - mrCar.pos->absY;
	
	dist.y = sqrtf(x * x + y * y);
	
	//DBG("Dist %lld", dist.y);
}

void startPID()
{	
//	commitParam(&angle);
//	commitParam(&dist);
	PID_configTimer();
}

void pidF()
{	
	pidFlag = 0;
}

void stopPID()
{
	pidFlag = 0;
}
