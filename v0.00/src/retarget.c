#include <stdio.h>
#include "serie2.h"
#include "serie1.h"

struct __FILE {
  int handle;                 // Add whatever you need here 
};

FILE __stdout;
FILE __stdin;

FILE __wifiOut;
FILE __wifiIn;

/*----------------------------------------------------------------------------
  fputc
 *----------------------------------------------------------------------------*/
int fputc(int ch, FILE *f) {
  
	if (f == &__wifiOut)
		return ((int)_putchar1(ch));
	else
		return ((int)_putchar2(ch));
}

/*----------------------------------------------------------------------------
  fgetc
 *----------------------------------------------------------------------------*/
int fgetc(FILE *f) {
	
	if (f == &__wifiIn)
		return (_getkey1());
	else
		return (_putchar2(_getkey2()));
}

/*----------------------------------------------------------------------------
  _ttywrch
 *----------------------------------------------------------------------------*/
void _ttywrch(int ch) {
	//write to console
 _putchar2 (ch);
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

