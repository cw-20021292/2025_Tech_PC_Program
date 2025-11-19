/*
 * api_debug.h
 *
 *  
 *  
 */

#ifndef INC_API_DEBUG_H_
#define INC_API_DEBUG_H_

#include "api_uart.h"

/* Application Module */
enum {
	SYSMOD = 0, /* ETC */
	ADCMOD,		/* ADC debug */
	LEDMOD,		/* LED debug */
	KEYMOD,		/* KEY debug */
};

#define _OFF	0
#define _ON		1

/* Debug level */
#define FATAL	0
#define INFO	1
#define DATA	2
#define DBUG	3

#define DEBUG_CONSOLE_ENABLED	(_ON)
#define DEBUG_LVL				(DBUG)

#define DEBUG_MOD_SYSMOD        (1<<SYSMOD)
#define DEBUG_MOD_ADCMOD        (1<<ADCMOD)
#define DEBUG_MOD_LEDMOD        (1<<LEDMOD)
#define DEBUG_MOD_KEYMOD        (1<<KEYMOD)

#define DEBUG_MOD_ALL           ( DEBUG_MOD_SYSMOD | \
                                    DEBUG_MOD_ADCMOD| \
                                    DEBUG_MOD_LEDMOD | \
                                    DEBUG_MOD_KEYMOD)

#define DEBUG_MOD_DEF           ( DEBUG_MOD_SYSMOD )

/* Default All module debug enable */
#define DEBUG_MODULE 			DEBUG_MOD_DEF

#define C_RED			"\x1b[31m"
#define C_BLUE			"\x1b[34m"
#define C_YELLOW		"\x1b[33m"
#define C_GREEN			"\x1b[32m"
#define C_MAGENTA		"\x1b[35m"
#define C_RESET			"\x1b[0m"

#if (DEBUG_CONSOLE_ENABLED == _ON)
#define dlog(module, level, args) \
            do { \
                if ((DEBUG_LVL >= level) && (DEBUG_MODULE & (1 << module))) { \
                    if (level == FATAL) \
                        printf(C_RED "[FATAL] " C_RESET); \
                    else if (level == INFO) \
                        printf(C_YELLOW "[INFO] " C_RESET); \
                    else if (level == DATA) \
                        printf(C_MAGENTA "[DATA] " C_RESET); \
                    else if (level == DBUG) \
                        printf(C_GREEN "[DBUG] " C_RESET); \
                    \
                    printf args; \
                } \
            } while(0)
#else
#define dlog(module,level, fmt, ...)
#endif


#endif /* INC_API_DEBUG_H_ */
