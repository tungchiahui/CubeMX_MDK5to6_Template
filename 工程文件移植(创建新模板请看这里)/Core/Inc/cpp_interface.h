#ifndef __CPP_INTERFACE_H_
#define __CPP_INTERFACE_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include "main.h"
#include "struct_typedef.h"

/******************define_config********************/
//�Ƿ�Ϊ�������:
//������������������0��
//�����FreeRTOS����������1��
//�����UOS����������xxxx��(��������ش���)
#define isRTOS 0

#if isRTOS == 1
#include "cmsis_os.h"
#endif


	
void cpp_main(void);
	
	

#ifdef __cplusplus
}
#endif
	
#endif
