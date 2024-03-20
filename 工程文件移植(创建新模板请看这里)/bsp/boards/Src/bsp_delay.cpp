#include "bsp_delay.h"

#if isRTOS == 1
    #include "cmsis_os.h"
#endif

static uint32_t g_fac_us = 0;       /* us延时倍乘数 */

/*******对象*******/
BSP_Delay bsp_delay;

/**
 * @brief       初始化延迟函数
 * @param       sysclk: 系统时钟频率, 即CPU频率(HCLK)
 * @retval      无
 */
 void BSP_Delay::F1::Init(uint16_t sysclk)
{
    SysTick->CTRL = 0;                                          /* 清Systick状态，以便下一步重设，如果这里开了中断会关闭其中断 */
    HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK_DIV8);   /* SYSTICK使用内核时钟源8分频,因systick的计数器最大值只有2^24 */

    g_fac_us = sysclk / 8;                                      /* 不论是否使用OS,g_fac_us都需要使用,作为1us的基础时基 */
}

/**
 * @brief       延时nus
 * @param       nus: 要延时的us数.
 * @note        注意: nus的值,不要大于1864135us(最大值即2^24 / g_fac_us  @g_fac_us = 9)
 * @retval      无
 */
void BSP_Delay::F1::us(uint32_t nus)
{
    uint32_t temp;
    SysTick->LOAD = nus * g_fac_us; /* 时间加载 */
    SysTick->VAL = 0x00;            /* 清空计数器 */
    SysTick->CTRL |= 1 << 0 ;       /* 开始倒数 */

    do
    {
        temp = SysTick->CTRL;
    } while ((temp & 0x01) && !(temp & (1 << 16))); /* CTRL.ENABLE位必须为1, 并等待时间到达 */

    SysTick->CTRL &= ~(1 << 0) ;    /* 关闭SYSTICK */
    SysTick->VAL = 0X00;            /* 清空计数器 */
}

/**
 * @brief       延时nms
 * @param       nms: 要延时的ms数 (0< nms <= 65535)
 * @retval      无
 */
void BSP_Delay::F1::ms(uint16_t nms)
{
    uint32_t repeat = nms / 1000;   /*  这里用1000,是考虑到可能有超频应用,
                                     *  比如128Mhz的时候, delay_us最大只能延时1048576us左右了
                                     */
    uint32_t remain = nms % 1000;

    while (repeat)
    {
        us(1000 * 1000);      /* 利用delay_us 实现 1000ms 延时 */
        repeat--;
    }

    if (remain)
    {
        us(remain * 1000);    /* 利用delay_us, 把尾数延时(remain ms)给做了 */
    }
}



/**
 * @brief     初始化延迟函数
 * @param     sysclk: 系统时钟频率, 即CPU频率(rcc_c_ck), 168MHz
 * @retval    无
 */  
void BSP_Delay::F4::Init(uint16_t sysclk)
{
    HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);/* SYSTICK使用外部时钟源,频率为HCLK */
    g_fac_us = sysclk;                                  /* 不论是否使用OS,g_fac_us都需要使用 */
}


/**
 * @brief       延时nus
 * @param       nus: 要延时的us数.
 * @note        nus取值范围 : 0~190887435(最大值即 2^32 / fac_us @fac_us = 21)
 * @retval      无
 */
void BSP_Delay::F4::us(uint32_t nus)
{
    uint32_t ticks;
    uint32_t told, tnow, tcnt = 0;
    uint32_t reload = SysTick->LOAD;        /* LOAD的值 */
    ticks = nus * g_fac_us;                 /* 需要的节拍数 */
    told = SysTick->VAL;                    /* 刚进入时的计数器值 */
    while (1)
    {
        tnow = SysTick->VAL;
        if (tnow != told)
        {
            if (tnow < told)
            {
                tcnt += told - tnow;        /* 这里注意一下SYSTICK是一个递减的计数器就可以了 */
            }
            else 
            {
                tcnt += reload - tnow + told;
            }
            told = tnow;
            if (tcnt >= ticks)
            {
                break;                      /* 时间超过/等于要延迟的时间,则退出 */
            }
        }
    }
}

/**
 * @brief       延时nms
 * @param       nms: 要延时的ms数 (0< nms <= 65535)
 * @retval      无
 */
void BSP_Delay::F4::ms(uint16_t nms)
{
    uint32_t repeat = nms / 540;    /*  这里用540,是考虑到可能有超频应用, 比如248M的时候,delay_us最大只能延时541ms左右了 */
    uint32_t remain = nms % 540;

    while (repeat)
    {
        us(540 * 1000);        /* 利用delay_us 实现 540ms 延时 */
        repeat--;
    }

    if (remain)
    {
        us(remain * 1000);    /* 利用delay_us, 把尾数延时(remain ms)给做了 */
    }
}

void BSP_Delay::FreeRTOS::Init(void)
{
	//调用FreeRTOS自带的延时即可。
	//osDelay
	//vTaskDelay
	//vTaskDelayUntil
}


extern "C"
/**
  * @brief HAL库内部函数用到的延时
           HAL库的延时默认用Systick，如果我们没有开Systick的中断会导致调用这个延时后无法退出
  * @param Delay 要延时的毫秒数
  * @retval None
  */
void HAL_Delay(uint32_t Delay)
{
#if isRTOS==0   //如果是裸机开发
	
	#ifdef STM32F1  //如果是裸机开发且为F1
			bsp_delay.f1.ms(Delay);
	#endif
	
	#ifdef STM32F4  //如果是裸机开发且为F4
			bsp_delay.f4.ms(Delay);
	#endif
	
#elif isRTOS==1          //如果是FreeRTOS开发
		 osDelay(Delay);
#endif
}
