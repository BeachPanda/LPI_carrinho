#include "stm32f4xx_rcc.h"
#include "stm32f4xx_tim.h"

typedef enum {autom, manual}mode;

typedef struct variable
{
	int max;
	int min;
	float value;
	float kh;
}k; //ganho

typedef struct output
{
	int max;
	int min;
	float value;
	float ant;
	float o0;//offset
}out;

typedef struct error
{
	float sum;
	float ant;
}err;

typedef struct filter
{
	int N;
	float kf;
}filtr;

struct PID
{
	k ki;
	k kp;
	k kd;
	out u;
	err e;

	filtr filter;

	float ref;
	long long int y;

	float freq;

	mode mod;
};

typedef struct PID PID;

void PID_setFreq(float freq);
float pid_algor(PID *pid);
void PID_configTimer(void);
void commitParam(PID *pid);
void printPIDSettings(PID pid);
void initPID(PID *pid);
