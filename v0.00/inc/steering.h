
#ifndef CONTROLOPID_H_
#define CONTROLOPID_H_
#include "motorControl.h"
#include <stdio.h>
#include "arm_math.h"

typedef enum {autom, manual}mode;

#define N_POINTS 1000

struct points
{
	uint64_t x[N_POINTS];
	uint64_t y[N_POINTS];
	unsigned int start;	
	unsigned int end;	
};

typedef struct points points;

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

extern PID xcoor;
extern PID ycoor;

void initPID(PID *pid);
void startPID(void);
void stopPID(void);
void commitParam(void);
void PID_setFreq(float freq);
void pidF(void);

void printPIDSettings(PID pid);

void initSteering(void);

void addPoint(uint64_t x, uint64_t y);

extern char pidFlag;

#endif /* CONTROLOPID_H_ */
