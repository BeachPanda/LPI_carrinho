#include "ESP8266.h"

extern FILE __wifiOut, __wifiIn;

void initESP(void)
{
	usart1_init();

	_putchar1(0);
	ESPO("\n\r");
	ESPO("AT\n\r");
	
	sendData("LOOOOOOOOOOL");
}

void sendData(char *str)
{
	char c;
	
	ESPO ("AT+CIPSEND=0, 255\n\r");
	ESPI ("%c", &c);
	
	if(c == '>')
		ESPO ("%s\n\r", str);
}
