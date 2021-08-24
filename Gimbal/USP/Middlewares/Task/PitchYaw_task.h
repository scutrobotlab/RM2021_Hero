#ifndef __PITCHYAW_TASK_H
#define __PITCHYAW_TASK_H
#ifdef __cplusplus



#include "System_DataPool.h"



extern float pit_ang;
extern float yaw_ang;



void pitchyaw_Init();
void pitchyaw_task(void *pvParameters);



#endif

#endif	/*__PITCHYAW_TASK_H */