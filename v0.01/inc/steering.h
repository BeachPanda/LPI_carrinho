
#ifndef CONTROLOPID_H_
#define CONTROLOPID_H_
#include "motorControl.h"
#include <stdio.h>
#include "arm_math.h"
#include "pid.h"

#define N_POINTS 100

struct points
{
	int64_t x[N_POINTS];
	int64_t y[N_POINTS];
	volatile unsigned int start;	
	volatile unsigned int end;	
};

typedef struct points points;

extern PID dist;
extern PID angle;

void startPID(void);
void stopPID(void);

void pidF(void);

void printPIDSettings(PID pid);

void initSteering(void);

void addPoint(int64_t x, int64_t y);
void switchCurrentPoint(int64_t x, int64_t y);

void getDistRef(void);
void getAngleRef(void);

void loadRoute(void);

extern char pidFlag;

#endif /* CONTROLOPID_H_ */
