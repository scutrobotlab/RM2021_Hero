#ifndef __MOVE_TASK_H
#define __MOVE_TASK_H
#ifdef __cplusplus

#include "FreeRTOS.h"
#include "task.h"
#include "System_DataPool.h"
#include "Motor.h"
#include "Hero_chassis.h"





extern float pow_P;
extern float pow_I;
extern float pow_D;
extern float max_out;


void move_task_Init();






#endif
#endif /*__MOVE_TASK_H */