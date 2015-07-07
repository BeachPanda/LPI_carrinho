/*
 * serie.h
 *
 *  Created on: Feb 19, 2015
 *      Author: david
 */

#ifndef SERIE1_H_
#define SERIE1_H_

//#include "stm32f1xx.h"
#include "stm32f4xx_usart.h"

volatile extern unsigned int istart1;
volatile extern unsigned int iend1;

volatile extern unsigned int ostart1;
volatile extern unsigned int oend1;

void usart1_init(void);

char _getkey1(void);

char _putchar1(char c);

void putString1(char *c);

void putbuf1(char c);

#define isInBEmpty1()(istart1 == iend1)
#define isOutBEmpty1() (ostart1 == oend1)

#endif /* SERIE_H_ */
