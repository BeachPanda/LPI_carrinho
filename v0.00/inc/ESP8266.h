
#include "serie1.h"
#include <stdio.h>

#define wifiOut (&__CLIBNS __wifiOut)
#define wifiIn (&__CLIBNS __wifiIn)

//send wireless message
#define	ESPO(...) fprintf(wifiOut, __VA_ARGS__);
//read wireless message
#define	ESPI(...) fscanf(wifiIn, __VA_ARGS__); 

void initESP(void);

void sendData(char *str);
