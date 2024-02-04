#ifndef __STARTUP_MAIN_H_
#define __STARTUP_MAIN_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include "main.h"
#include "struct_typedef.h"

/******************define_config********************/
//是否为裸机开发:
//如果是裸机开发，则填0。
//如果是FreeRTOS开发，则填1。
//如果是UOS开发，则填xxxx。(待开发相关代码)
#define isRTOS 0
	

void startup_main(void);
	
	

#ifdef __cplusplus
}
#endif
	
#endif
