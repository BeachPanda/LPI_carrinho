/*
 * serie.h
 *
 *  Created on: Feb 19, 2015
 *      Author: david
 */

#ifndef SERIE3_H_
#define SERIE3_H_

//#include "stm32f4xx.h"
#include "stm32f4xx_usart.h"
#include <stdio.h>


extern volatile unsigned int istart3;
extern volatile unsigned int iend3;

extern volatile unsigned int ostart3;
extern volatile unsigned int oend3;

void usart3_init (void);

char _getkey3(void);

char _putchar3(char c);

void putString3(char *c);

void putbuf3 (char c);

#define isInBEmpty3() (istart3 == iend3)
#define isOutBEmpt3() (ostart3 == oend3)

#endif /* SERIE_H_ */
