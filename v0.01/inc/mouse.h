#ifndef _MOUSE_H
#define _MOUSE_H

#include "stdint.h"

struct  mouse{
	int8_t diffX;
	int8_t diffY;
	int64_t absX;
	int64_t absY;
	float teta;
};

typedef struct mouse mouse;

extern mouse mouz;

#endif
