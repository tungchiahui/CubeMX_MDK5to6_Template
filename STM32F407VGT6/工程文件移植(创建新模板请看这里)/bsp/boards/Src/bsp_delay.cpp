#include "bsp_delay.h"

#if isRTOS == 1
    #include "cmsis_os.h"
#endif

static uint32_t g_fac_us = 0;       /* us��ʱ������ */

BSP_Delay bsp_delay;

/**
 * @brief       ��ʼ���ӳٺ���
 * @param       sysclk: ϵͳʱ��Ƶ��, ��CPUƵ��(HCLK)
 * @retval      ��
 */
 void BSP_Delay::F1::Init(uint16_t sysclk)
{
    SysTick->CTRL = 0;                                          /* ��Systick״̬���Ա���һ�����裬������￪���жϻ�ر����ж� */
    HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK_DIV8);   /* SYSTICKʹ���ں�ʱ��Դ8��Ƶ,��systick�ļ��������ֵֻ��2^24 */

    g_fac_us = sysclk / 8;                                      /* �����Ƿ�ʹ��OS,g_fac_us����Ҫʹ��,��Ϊ1us�Ļ���ʱ�� */
}

/**
 * @brief       ��ʱnus
 * @param       nus: Ҫ��ʱ��us��.
 * @note        ע��: nus��ֵ,��Ҫ����1864135us(���ֵ��2^24 / g_fac_us  @g_fac_us = 9)
 * @retval      ��
 */
void BSP_Delay::F1::us(uint32_t nus)
{
    uint32_t temp;
    SysTick->LOAD = nus * g_fac_us; /* ʱ����� */
    SysTick->VAL = 0x00;            /* ��ռ����� */
    SysTick->CTRL |= 1 << 0 ;       /* ��ʼ���� */

    do
    {
        temp = SysTick->CTRL;
    } while ((temp & 0x01) && !(temp & (1 << 16))); /* CTRL.ENABLEλ����Ϊ1, ���ȴ�ʱ�䵽�� */

    SysTick->CTRL &= ~(1 << 0) ;    /* �ر�SYSTICK */
    SysTick->VAL = 0X00;            /* ��ռ����� */
}

/**
 * @brief       ��ʱnms
 * @param       nms: Ҫ��ʱ��ms�� (0< nms <= 65535)
 * @retval      ��
 */
void BSP_Delay::F1::ms(uint16_t nms)
{
    uint32_t repeat = nms / 1000;   /*  ������1000,�ǿ��ǵ������г�ƵӦ��,
                                     *  ����128Mhz��ʱ��, delay_us���ֻ����ʱ1048576us������
                                     */
    uint32_t remain = nms % 1000;

    while (repeat)
    {
        us(1000 * 1000);      /* ����delay_us ʵ�� 1000ms ��ʱ */
        repeat--;
    }

    if (remain)
    {
        us(remain * 1000);    /* ����delay_us, ��β����ʱ(remain ms)������ */
    }
}



/**
 * @brief     ��ʼ���ӳٺ���
 * @param     sysclk: ϵͳʱ��Ƶ��, ��CPUƵ��(rcc_c_ck), 168MHz
 * @retval    ��
 */  
void BSP_Delay::F4::Init(uint16_t sysclk)
{
    HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);/* SYSTICKʹ���ⲿʱ��Դ,Ƶ��ΪHCLK */
    g_fac_us = sysclk;                                  /* �����Ƿ�ʹ��OS,g_fac_us����Ҫʹ�� */
}


/**
 * @brief       ��ʱnus
 * @param       nus: Ҫ��ʱ��us��.
 * @note        nusȡֵ��Χ : 0~190887435(���ֵ�� 2^32 / fac_us @fac_us = 21)
 * @retval      ��
 */
void BSP_Delay::F4::us(uint32_t nus)
{
    uint32_t ticks;
    uint32_t told, tnow, tcnt = 0;
    uint32_t reload = SysTick->LOAD;        /* LOAD��ֵ */
    ticks = nus * g_fac_us;                 /* ��Ҫ�Ľ����� */
    told = SysTick->VAL;                    /* �ս���ʱ�ļ�����ֵ */
    while (1)
    {
        tnow = SysTick->VAL;
        if (tnow != told)
        {
            if (tnow < told)
            {
                tcnt += told - tnow;        /* ����ע��һ��SYSTICK��һ���ݼ��ļ������Ϳ����� */
            }
            else 
            {
                tcnt += reload - tnow + told;
            }
            told = tnow;
            if (tcnt >= ticks)
            {
                break;                      /* ʱ�䳬��/����Ҫ�ӳٵ�ʱ��,���˳� */
            }
        }
    }
}

/**
 * @brief       ��ʱnms
 * @param       nms: Ҫ��ʱ��ms�� (0< nms <= 65535)
 * @retval      ��
 */
void BSP_Delay::F4::ms(uint16_t nms)
{
    uint32_t repeat = nms / 540;    /*  ������540,�ǿ��ǵ������г�ƵӦ��, ����248M��ʱ��,delay_us���ֻ����ʱ541ms������ */
    uint32_t remain = nms % 540;

    while (repeat)
    {
        us(540 * 1000);        /* ����delay_us ʵ�� 540ms ��ʱ */
        repeat--;
    }

    if (remain)
    {
        us(remain * 1000);    /* ����delay_us, ��β����ʱ(remain ms)������ */
    }
}

void BSP_Delay::FreeRTOS::Init(void)
{
	//����FreeRTOS�Դ�����ʱ���ɡ�
	//osDelay
	//vTaskDelay
	//vTaskDelayUntil
}



/**
  * @brief HAL���ڲ������õ�����ʱ
           HAL�����ʱĬ����Systick���������û�п�Systick���жϻᵼ�µ��������ʱ���޷��˳�
  * @param Delay Ҫ��ʱ�ĺ�����
  * @retval None
  */
void HAL_Delay(uint32_t Delay)
{
#if isRTOS==0   //������������
	
	#ifdef STM32F1  //��������������ΪF1
			bsp_delay.f1.ms(Delay);
	#endif
	
	#ifdef STM32F4  //��������������ΪF4
			bsp_delay.f4.ms(Delay);
	#endif
	
#elif isRTOS==1          //�����FreeRTOS����
		 osDelay(Delay);
#endif
}
