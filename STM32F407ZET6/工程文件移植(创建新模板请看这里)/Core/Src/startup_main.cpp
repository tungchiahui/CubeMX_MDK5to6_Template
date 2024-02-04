#include "startup_main.h"


void startup_main(void)
{

	
#if isRTOS==0    	//如果是裸机开发
	for(;;)  //等同于while(true)
	{

	}
#endif
}