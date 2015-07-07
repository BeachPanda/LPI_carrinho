#include "interface.h"
#include "debug.h"

const char allowedSymbols[] = {'.', '?', '$', ' ','-', BCKSPC, ESC, END1, END2, 0};

/***********************
*struct
*
***********************/

extern car mrCar;

const struct command commandVec[] =
{
	{
		"AP",
		"ADD POINT\n"
		"AP X?? Y??\n",
		_addPoint
	},
	{
		"CP",
		"CHECK POSITION\n"
		"CP\n",
		_checkPos
	},
	{
		"RP",
		"REPORT\n"
		"RP\n",
		_report
	},
	{
		"SCP",
		"SWITCH CURRENT POINT"
		"SCP X?? Y??\n",
		_switchCurrentPoint
	},
	{
		"SM",
		"SELECT MODE - JOYSTICK/ROUTED\n"
		"SM J|R\n",
		_selectMode
	},
	{
		"PH",
		"PLAY HORN\n"
		"SM\n",
		_playHorn
	},
	{
	 "?",
	 "HELP - DISPLAYS THIS HELP\n"\
	 "?\n",
	 _help
	},
	{
	 "",
	 "",
	 NULL
	}
};

char _report(char *str)
{
	sendReport(&mrCar);
	return SUCCESS;
}

char _checkPos(char *str)
{
	sendPos(&mrCar);
	return SUCCESS;
}

char _addPoint(char *str)
{
	uint64_t x, y;
	int n = 0;
	
	n = sscanf(str, "AP X %lld Y %lld", &x, &y);
	
	if(n < 2)
		return ERROR;
	
	addPoint(x, y);
	
	return SUCCESS;
}

char _switchCurrentPoint(char *str)
{
	uint64_t x, y;
	int n = 0;
	
	n = sscanf(str, "SCP X %lld Y %lld", &x, &y);
	
	if(n < 2)
		return ERROR;
	
	switchCurrentPoint(x, y);
	
	return SUCCESS;
}

char _selectMode(char *str)
{
	int mode;
	int n;
	
	n = sscanf(str, "SM %d", &mode);
	
	if(n > 0){
		//selectMode(mode)
		return SUCCESS;
	}
	
	return ERROR;
}

char _playHorn(char *str)
{
	playHorn();
	return SUCCESS;
}

//ESCAPE
//Clear all characters
void esc(char* buff, int i)
{
	while(i > 0){
		putchar(BCKSPC);
		buff[i--] = 0;
	}
}

//is letter
//returns 1 if it is
char isL(char a)
{
	if((a >= 'a') && (a <= 'z')) return 1;
	if((a >= 'A') && (a <= 'Z')) return 1;
	return 0;
}

//is number
//returns 1 if it is
char isN(char a)
{
	if((a < '0') || (a > '9')) return 0;
	return 1;
}

//is allowed symbol
//returns 1 if it is
char isS(char a)
{
	int i = 0;

	while(allowedSymbols[i])
	{
		if(allowedSymbols[i] == a) return 1;
		i++;
	}

	return 0;
}

//is character valid
//returns 1 if it is
int isCharValid(char a)
{
	if(isN(a)) return 1; // if it's a number
	else if(isL(a)) return 1;// or a letter
	else if(isS(a)) return 1;// or an allowed symbol

	return 0;
}

char promptInput(char buff[2][BUFFER_SIZE], char* input)
{
	unsigned char temp;
	char flag;
	static int i = 0;

	flag = UNDEFINED;

	temp = _getkey1(); //guarda caracter recebido

	if((i >= (BUFFER_SIZE - 1)) && (temp != BCKSPC)){
		buff[0][i] = 0;
		printf("\nWARNING! buffer full!\n");
		_putchar1(PROMPT);
		printf("%s", input);
	}
	else if(!isCharValid(temp)){
		flag = INVALID;
		printf("0x%x\n", temp);
	}
	else if(temp == BCKSPC)
	{
		//printf("Debug i:%d\n", i);
		if(i > 0){
			_putchar1(BCKSPC);
			buff[0][i] = 0;
			i--;
		}
	}
	else if(temp == ESC)
	{
		esc(buff[0], i);
		i = 0;
	}
	else if(temp == '$')
	{
		if(buff[1][0] == 0)
		{
			printf("\nType a valid command first!");
			flag = REPEAT;
		}
		else
		{
			esc(buff[0], i);
//			strcpy(buff[0], buff[1]);
//			i = strlen(buff[0]);
			printf("%s", input);
			//printf("Debug i:%d\n", i);
		}
	}
	else if((temp == END1) || (temp == END2))
	{
		(buff[0][i]) = 0;
		//printf_P(PSTR("\nDebug buff:%s-%s.", buff[0], buff[1]);
		flag = VALID;
		i = 0;
		//_putchar('4');
	}
	else{
		(buff[0][i]) = temp;
		_putchar1(temp);
		i++;
	}

	if(flag != UNDEFINED) i = 0;

	return flag;
}

//returns command index
int getCommand(char **c)
{
	int i = 0;
	char *a = *c;
	char ins[5];

	//printf("\n%s", *c);

	while((i < 5) && (*(a) != ' ') && (*(a) != '\0')){
		ins[i++] = *a++;
	}

	if(i != 0) ins[i] = 0;

	//printf("\n%s.%d", *c, i);

	i = 0;

	while(commandVec[i].instruction[0] != '\0') // identify it
	{
		//printf("Debug byte:%c\n", car);
		//printf("%S-%s.\n", &commandVec[i].instruction[0], ins);
		if(!strcmp(ins, &commandVec[i].instruction[0])) 
			return i;
		i++;
	}

	*c = --a;

	return -1;
}

//converts a series of 1's, 0's and ' ''s to an integer
//a hold the argument
//returns 1 if conversion successfull
char stringToBin(char **str, unsigned long int *a)
{
	unsigned int temp;
	char tempC;
	char *str1 = *str;
	temp = 0;

	//printf("\nDebug, str:%s", str);

	if((*str1 != '0') && (*str1 != '1') && (*str1 != ' ')) return ERROR;

	while((*str1 == '0') || (*str1 == '1') || (*str1 == ' ')){
		tempC = *(str1++);
		tempC -= '0';
		if((tempC == 0) || (tempC == 1))
		{
		    temp <<= 1;
			temp += tempC;
		}
	}

	*a = temp;

	*str = str1;
	return SUCCESS;
}

//returns successfull read args
//arg holds the argument itself
char getArgument(char** str, unsigned long int* arg)
{
	char *str1 = *str;
	char tempC;

	do{
		tempC = (*(str1++));
	}
	while((tempC != ' ') &&
		  (tempC != 0)   &&
		  (tempC != '.')); // lê um espaço ou um '\0' ou um '.'

	if(tempC == 0) return 0; // se '\0' termina

	do{
		tempC = (*str1++);
		if(isN(tempC)){
			tempC = 'D';
			str1--;
		}
	}while((tempC != 'H') &&
		   (tempC != 'B') &&
		   (tempC != 'D') &&
		   (*(str1)));

	switch(tempC)
	{
	case 'H':
		tempC = sscanf(str1, "%lX", arg);
		break;

	case 'D':
		tempC = sscanf(str1, "%lu", arg);
		break;

	case 'B':
		tempC = stringToBin(&str1, arg);
		break;
	default: return 0;
	}

	while((*str1 != ' ') && (*str1 != '\0') && (*str1 != '.')) str1++;
	*str = str1;

	return tempC;
}

//Displays help
char _help(char *str)
{
	int i = 0;

	//printf("\n%s", commandVec[12].help);

	while(commandVec[i].help[0] != '\0'){
		//printf("%02X; ", commandVec[i].help[0]);
		printf("%s\n", commandVec[i].help);
		i++;
	}
	return SUCCESS;
}

//Prints firmware version
char _version(char *str)
{
	printf("Versão 0.1\n");
	return SUCCESS;
}
