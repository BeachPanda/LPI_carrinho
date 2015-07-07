#include "ESP8266.h"

extern FILE __wifiOut, __wifiIn;

void ESPinit(void)
{
	usart1_init();

	_putchar1(0);
	ESPO("\nAT\n");
}

void sendData(char *str)
{
	char c;
	
	ESPO ("AT+CIPSEND=0, 255\n\r");
	c = ESPgetkey();
	
	if(c == '>')
		ESPO ("%s\n", str);
}
