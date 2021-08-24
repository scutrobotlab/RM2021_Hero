#include "move_task.h"

TaskHandle_t move_Task_Handler;

float pow_P;
float pow_I;
float pow_D;

extern hero_chassis Hero_chassis;

void move_task(void *pvParameters);
float max_out=0.0;
void move_task_Init(){
	xTaskCreate(move_task, "task_move", Small_Stack_Size, NULL, PriorityHigh, &move_Task_Handler);
	Hero_chassis.LF.SetPIDParam(15,0,0,15000,10000);	
	Hero_chassis.RF.SetPIDParam(15,0,0,15000,10000);
	Hero_chassis.LB.SetPIDParam(15,0,0,15000,10000);
	Hero_chassis.RB.SetPIDParam(15,0,0,15000,10000);
	chassis_power_ctrl.SetPIDParam(50,2000,0,15000.0f,20000.0f);
	chassis_power_ctrl.I_SeparThresh=20000;
	capCtrl.SetPIDParam(4.0f, 0, 0, 200, 800); 
}




uint16_t count=0;
void move_task(void *pvParameters){
	
	TickType_t xLastWakeTime_t;
    xLastWakeTime_t = xTaskGetTickCount();
	Hero_chassis.power_switch();
	for(;;){
		if(HAL_GPIO_ReadPin(Dial_switch_GPIO_Port,Dial_switch_Pin)==GPIO_PIN_SET){
			count++;
		}
		
		Sent_Contorl(&huart1);
		c_BoardCom.Send(&Referee);
		Hero_chassis.control();
		MotorMsgSend(&hcan2,wheel_Motor);
		vTaskDelayUntil(&xLastWakeTime_t, 1);
		
	}
}