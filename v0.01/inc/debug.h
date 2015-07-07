#ifndef _DEBUG_H
#define _DEBUG_H

#include <stdio.h>
#include "serie3.h"
#include "serie2.h"
#include "serie1.h"

#define dPutchar _putchar1
#define dGetKey _getkey1

	#ifdef DEBUG1 //Data
		#define	DBG1(...) {dPutchar('d' | 0x80); printf(__VA_ARGS__); dPutchar('d' | 0x80); dPutchar('\n');}
	#else
		#define DBG1(...)
	#endif

	#ifdef DEBUG0
		#define	DBG(...) {dPutchar('!' | 0x80); printf(__VA_ARGS__); dPutchar('!' | 0x80);  dPutchar('\n');}
	#else
		#define DBG(...)
	#endif

#endif
