#include "Hero_config.h"
#include "PitchYaw_task.h"
#include "Motor.h"
#include "mpu6050_config.h"
#include <UpperMonitor.h>
#include "FreeRTOS.h"
#include "task.h"

TaskHandle_t pitchyawTask_Handler;
float pit_ang=0;
float yaw_ang=0;

extern pitchyaw Pitchyaw;

void pitchyaw_task(void *pvParameters);


void pitchyaw_Init(){
	xTaskCreate(pitchyaw_task, "task_pitchyaw", Small_Stack_Size, NULL, PriorityHigh, &pitchyawTask_Handler);
}





void pitchyaw_task(void *pvParameters){
	TickType_t xLastWakeTime_t;
    xLastWakeTime_t = xTaskGetTickCount();
	vTaskDelay(3000);	//µÈ´ýÍÓÂÝÒÇÎÈ¶¨
	
	for(;;){
		Sent_Contorl(&huart1);
		

		pit_ang=Pitchyaw.compare(90,-100,pit_ang);
		Pitchyaw.control(pit_ang,yaw_ang);
		vTaskDelayUntil(&xLastWakeTime_t, 1);
	}

}