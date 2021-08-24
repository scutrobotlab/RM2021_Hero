#include "Shoot_task.h"
#include "Hero_config.h"


TaskHandle_t ShotTask_Handler;







void shoot_task(void *pvParameters);
void shoot_Init(){
	xTaskCreate(shoot_task, "task_shoot", Small_Stack_Size, NULL, PriorityHigh, &ShotTask_Handler);

}


void shoot_task(void *pvParameters){
    TickType_t xLastWakeTime_t;
    xLastWakeTime_t = xTaskGetTickCount();
	for(;;){
		DR16.Check_Link(xTaskGetTickCount());	//暂时放在这里
		Shoot.control();
//		
		vTaskDelayUntil(&xLastWakeTime_t, 1);
	}
}