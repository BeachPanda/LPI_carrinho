#ifndef INTERFACE_H_
#define INTERFACE_H_

#include "serie2.h"
#include "car.h"
#include <string.h>

//#ifndef SUCCESS
//#define SUCCESS 1
//#endif
//#ifndef ERROR
//#define ERROR 0
//#endif

#define BUFFER_SIZE 50

#define BCKSPC 0X08
#define ESC    0x1B
#define PROMPT '#'
#define END1 0x0D
#define END2 0x0A

#define VALID 1
#define INVALID 2
#define REPEAT 3
#define UNDEFINED 0

struct command{
	const char instruction[5];
	const char help[120];
	char (*func)(char*);
};

extern const char allowedSymbols[];
extern const struct command commandVec[];

//typedef enum{ERROR = 0, SUCCESS} ErrorLevel;


/************************
 * parsing and input
 *
 ***********************/
int getCommand(char **str);

char getArgument(char **str, unsigned long int* );

char promptInput(char buff[2][BUFFER_SIZE], char* input);

/***********************
*	GENERAL
*
*************************/
char _addPoint(char *str);
char _checkPos(char *str);
char _report(char *str);
char _selectMode(char *str);
char _switchCurrentPoint(char *str);
char _playHorn(char *str);
char _help(char *str);



#endif /* INTERFACE_H_ */
