#ifndef __HERO_GIMBAL_H
#define __HERO_GIMBAL_H
#ifdef __cplusplus

#include "shoot.h"
#include "pitchyaw.h"
#include "Chassis_control.h"
#include "PCvision.h"
#include "Indicator.h"


class Gimbal{
	public:
		
		float pitch_target = 0.0;
	
		shoot Shoot;
		C_Chassis Chassis;
		PCvision_Classdef PCvision;
		pitchyaw Pitchyaw;
		Indicator_Classdef indicator;		//新版状态指示灯
	
				/*CAN接收数据更新*/
		void CAN1_MotorUpdate(CAN_COB* CAN_RxMsg);
		void CAN2_MotorUpdate(CAN_COB* CAN_RxMsg);

	
		void Gimbal_Init();
		void control();
		void update();
		void stop();
	
		/* 利用构造函数初始化状态指示灯 */
		Gimbal():indicator(&htim2, &hdma_tim2_ch2_ch4, TIM_CHANNEL_2){};
	private:
			/* 电机的数据包 */
		Motor_CAN_COB CAN1_pack;
		Motor_CAN_COB CAN2_pack;
			/*CAN发送数据处理*/
		void Can_pack_process();
};


extern Gimbal Hero_Gimbal;

#endif

#endif	/*__HERO_GIMBAL_H */