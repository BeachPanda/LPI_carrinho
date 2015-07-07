
#include "serie1.h"
#include <stdio.h>

#define wifiOut (&__CLIBNS __wifiOut)
#define wifiIn (&__CLIBNS __wifiIn)

//send wireless message
#define	ESPO(...) fprintf(wifiOut, __VA_ARGS__);
//read wireless message
#define	ESPI(...) fscanf(wifiIn, __VA_ARGS__);
#define ESPgetkey() _getkey1();

struct esp8266
{
	uint8_t mode;
	char ssid[20];
	char psswd[20];
	uint8_t channel;
};

void ESPinit(void);

void sendData(char *str);
