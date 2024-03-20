#ifndef __BSP_DELAY_H_
#define __BSP_DELAY_H_

#ifdef __cplusplus
extern "C" 
{
#endif

#include "startup_main.h"

class BSP_Delay
{
	public:
		class F1
		{
			public:
				void Init(uint16_t sysclk);
				void us(uint32_t nus);
				void ms(uint16_t nms);
		}f1;
		class F4
		{
			public:
				void Init(uint16_t sysclk);
				void us(uint32_t nus);
				void ms(uint16_t nms);
		}f4;
		class FreeRTOS
		{
			public:
				void Init(void);
		}freertos;
};
/*******对象*******/
extern BSP_Delay bsp_delay;

#ifdef __cplusplus
}
#endif

#endif
