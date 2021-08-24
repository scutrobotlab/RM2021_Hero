#ifndef __DR16_TASK_H
#define __DR16_TASK_H
#ifdef __cplusplus

#include "System_DataPool.h"



extern TaskHandle_t DR16Task_Handler;

void DR16_Init();
void Datapack_process();
void DR16_task(void *pvParameters);
#endif

#endif	/*__DR16_TASK_H */