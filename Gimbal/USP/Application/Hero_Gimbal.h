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
		Indicator_Classdef indicator;		//�°�״ָ̬ʾ��
	
				/*CAN�������ݸ���*/
		void CAN1_MotorUpdate(CAN_COB* CAN_RxMsg);
		void CAN2_MotorUpdate(CAN_COB* CAN_RxMsg);

	
		void Gimbal_Init();
		void control();
		void update();
		void stop();
	
		/* ���ù��캯����ʼ��״ָ̬ʾ�� */
		Gimbal():indicator(&htim2, &hdma_tim2_ch2_ch4, TIM_CHANNEL_2){};
	private:
			/* ��������ݰ� */
		Motor_CAN_COB CAN1_pack;
		Motor_CAN_COB CAN2_pack;
			/*CAN�������ݴ���*/
		void Can_pack_process();
};


extern Gimbal Hero_Gimbal;

#endif

#endif	/*__HERO_GIMBAL_H */