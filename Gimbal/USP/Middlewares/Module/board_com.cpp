#include "board_com.h"
#include "Chassis_control.h"
TaskHandle_t board_comTask_Handler;



void Board_com_task(void *arg);

void board_com_Init(){
	xTaskCreate(Board_com_task, "task_shoot", Small_Stack_Size, NULL, PriorityHigh, &board_comTask_Handler);
}




/**
 * @brief  板间通信任务，通过此任务不断给电流采样板发送数据
 * @param  none
 * @return none
 */
void Board_com_task(void *arg){
	TickType_t xLastWakeTime_t;
    xLastWakeTime_t = xTaskGetTickCount();
	for(;;){
		Chassis.chassis_follow(); 
		vTaskDelayUntil(&xLastWakeTime_t, 1);
	}


}