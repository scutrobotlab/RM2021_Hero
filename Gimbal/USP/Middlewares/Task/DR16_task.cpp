#include "DR16_task.h"
#include "DR16.h"


#define PITCH_ANGLE 0.3;
#define YAW_ANGLE 0.5;

TaskHandle_t DR16Task_Handler;

void DR16_task(void *pvParameters);

void DR16_Init(){
	xTaskCreate(DR16_task, "task_shoot", Small_Stack_Size, NULL, PriorityHigh, &DR16Task_Handler);
}

void Datapack_process(){
	static DR16_DataPack_Typedef* dr16_pack;
	BaseType_t xTaskWokenByReceive = pdFALSE;
	if(xQueueReceiveFromISR(DR16_QueueHandle, &dr16_pack, &xTaskWokenByReceive) == pdTRUE){
		/* Remote control data unpacking */
		DR16.DataCapture(dr16_pack);
	}
}

void DR16_task(void *pvParameters){
	TickType_t xLastWakeTime_t;
    xLastWakeTime_t = xTaskGetTickCount();
	for(;;){
		Datapack_process();
		if(DR16.GetStatus()==Connection_Established){
//			pit_ang=pit_ang-DR16.Get_RY_Norm()*PITCH_ANGLE;
//			yaw_ang=yaw_ang-DR16.Get_RX_Norm()*YAW_ANGLE;
//			x_target=DR16.Get_LX_Norm()*1000;
//			y_target=DR16.Get_LY_Norm()*1000;
//			r_target=-DR16.Get_RX_Norm()*1000;
//		}
//		if(DR16.GetS1()==DOWN){
//			r_target=-1000;
//		}
//		else if(DR16.GetStatus()==Connection_Lost){
////			pit_ang=-50;
////			yaw_ang=0;
//			x_target=0;
//			y_target=0;
//			r_target=0;
		}
		vTaskDelayUntil(&xLastWakeTime_t, 1);
	}
}