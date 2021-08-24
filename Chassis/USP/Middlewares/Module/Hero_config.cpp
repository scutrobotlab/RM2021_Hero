#include "Hero_config.h"
#include "UI_Task.h"

extern hero_chassis Hero_chassis;

referee_Classdef Referee;
myPID chassis_power_ctrl;	//功率限制环PID
myPID capCtrl;				//超级电容充电功率环PID
PowerCtrl_ClassDef PowerCtrl(4, REAL_POWER_LOOP, __ENABLE, 1);

uint32_t Get_RefereeTime()
{
	return xTaskGetTickCount()*1000;
}



void Hero_Config_Init(){
	UI_Init();
	move_task_Init();
	Referee.Init(&huart6, Get_RefereeTime);
	PowerCtrl.Load_motorLimitController(MotorLimitController);
	PowerCtrl.Load_capChargeController(CapChargeController);
}




/**
 * @brief  功率限制函数
 * @param  current:从裁判系统读取的当前功率
 * @param  target:限定的功率值
 * @return chassis_power_ctrl.Out:功率PID输出值
 */
float MotorLimitController(float current,float target){
	chassis_power_ctrl.Current=current;
	chassis_power_ctrl.Target=target;
	chassis_power_ctrl.Out=chassis_power_ctrl.Adjust();
	return chassis_power_ctrl.Out;
}


/**
 * @brief  电容充电功率控制函数
 * @param  current：能量缓存当前值 target：能量缓存目标值
 * @return capCtrl.Out
 */
float CapChargeController(const float current, const float target){
	capCtrl.Target = target;
	capCtrl.Current = current;
	capCtrl.Adjust();
	return capCtrl.Out;		
}


