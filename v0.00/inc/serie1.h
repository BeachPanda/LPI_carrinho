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

#ifdef SERIE_C_1

#define OLEN1 32 // size of serial transmission buffer
volatile unsigned char ostart1; // transmission buffer start index
volatile unsigned char oend1; // transmission buffer end index
volatile char outbuf1[OLEN1]; // storage for transmission buffer

#define ILEN1 32 // size of serial receiving buffer
volatile unsigned char istart1; // receiving buffer start index
volatile unsigned char iend1; // receiving buffer end index
volatile char inbuf1[ILEN1]; // storage for receiving buffer

volatile char sendfull1; // flag: marks transmit buffer full
volatile char sendactive1; // flag: marks transmitter active

#endif

#ifndef SERIE_C_1
volatile extern unsigned int istart1;
volatile extern unsigned int iend1;

volatile extern unsigned int ostart1;
volatile extern unsigned int oend1;
#endif

void usart1_init(void);

char _getkey1(void);

char _putchar1(char c);

void putString1(char *c);

void putbuf1(char c);

#define isInBEmpty1 (istart1 == iend1)
#define isOutBEmpty1 (ostart1 == oend1)

#endif /* SERIE_H_ */
