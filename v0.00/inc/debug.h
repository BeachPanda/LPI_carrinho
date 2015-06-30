
#ifndef __DEBUG_H__
#define __DEBUG_H__

#include <stdio.h>

#ifdef 	DEBUG
#define	DBG(...)  printf(__VA_ARGS__)
#else	  
#define	DBG(...)
#endif

#define	INFO(...)  printf(__VA_ARGS__)
#define	ERR(...)  printf(__VA_ARGS__)

#endif  /* __DEBUG_H__ */
