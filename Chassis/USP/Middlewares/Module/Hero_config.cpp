#include "Hero_config.h"
#include "UI_Task.h"

extern hero_chassis Hero_chassis;

referee_Classdef Referee;
myPID chassis_power_ctrl;	//�������ƻ�PID
myPID capCtrl;				//�������ݳ�繦�ʻ�PID
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
 * @brief  �������ƺ���
 * @param  current:�Ӳ���ϵͳ��ȡ�ĵ�ǰ����
 * @param  target:�޶��Ĺ���ֵ
 * @return chassis_power_ctrl.Out:����PID���ֵ
 */
float MotorLimitController(float current,float target){
	chassis_power_ctrl.Current=current;
	chassis_power_ctrl.Target=target;
	chassis_power_ctrl.Out=chassis_power_ctrl.Adjust();
	return chassis_power_ctrl.Out;
}


/**
 * @brief  ���ݳ�繦�ʿ��ƺ���
 * @param  current���������浱ǰֵ target����������Ŀ��ֵ
 * @return capCtrl.Out
 */
float CapChargeController(const float current, const float target){
	capCtrl.Target = target;
	capCtrl.Current = current;
	capCtrl.Adjust();
	return capCtrl.Out;		
}


