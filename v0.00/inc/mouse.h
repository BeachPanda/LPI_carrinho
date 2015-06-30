#include "stdint.h"

struct  mouse{
	uint8_t diffX;
	uint8_t diffY;
	uint64_t absX;
	uint64_t absY;	
};

typedef struct mouse mouse;

extern mouse mouz;
