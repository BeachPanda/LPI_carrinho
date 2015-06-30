/*
 * serie.h
 *
 *  Created on: Feb 19, 2015
 *      Author: david
 */

#ifndef SERIE2_H_
#define SERIE2_H_

//#include "stm32f4xx.h"
#include "stm32f4xx_usart.h"
#include <stdio.h>


extern volatile unsigned int istart2;
extern volatile unsigned int iend2;

extern volatile unsigned int ostart2;
extern volatile unsigned int oend2;

void uart2_init (void);

char _getkey2 (void);

char _putchar2 (char c);

void putString2(char *c);

void putbuf2 (char c);

#define isInBEmpty2() (istart2 == iend2)
#define isOutBEmpty2() (ostart2 == oend2)

#endif /* SERIE_H_ */
