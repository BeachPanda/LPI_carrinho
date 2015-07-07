#include <stdio.h>
#include "serie3.h"
#include "serie2.h"
#include "serie1.h"

struct __FILE {
  int handle;                 // Add whatever you need here 
};


#define dPutchar _putchar1
#define dGetKey _getkey1



FILE __wifiOut;
FILE __wifiIn;

FILE __joyStickOut;
FILE __joyStickIn;

FILE __stdout;
FILE __stdin;

/*----------------------------------------------------------------------------
  fputc
 *----------------------------------------------------------------------------*/
int fputc(int ch, FILE *f) {
		return ((int)dPutchar(ch));
}

/*----------------------------------------------------------------------------
  fgetc
 *----------------------------------------------------------------------------*/
int fgetc(FILE *f) {
		return (dGetKey());
}

/*----------------------------------------------------------------------------
  _ttywrch
 *----------------------------------------------------------------------------*/
void _ttywrch(int ch) {
	//write to console
 dPutchar (ch);
}

/*----------------------------------------------------------------------------
  ferror
 *----------------------------------------------------------------------------*/
int ferror(FILE *f) {
                              // Your implementation of ferror
  return EOF;
}

/*----------------------------------------------------------------------------
  _sys_exit
 *----------------------------------------------------------------------------*/
void _sys_exit(int return_code) {
label:  goto label;           // endless loop
}

