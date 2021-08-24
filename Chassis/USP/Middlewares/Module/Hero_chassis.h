#ifndef __HERO_CHASSIS_H
#define __HERO_CHASSIS_H
#ifdef __cplusplus


#include "stdint.h"
#include "PID.h"
#include "Motor.h"
#include "main.h"
#include "source_manage.h"
#include "System_DataPool.h"
#include "move_task.h"
#include "Hero_config.h"

#define SUPPLY_BY_BAT 1 /* ��ع��磬���Ƶ������ޣ���ֹ�������� */
#define INFANTRY_MAX_SPEED 9000		//��������ٶȣ��˶�����
#if SUPPLY_BY_BAT
	#define INFANTRY_MAX_CURRENT 7000	//���������ٶ�
#else
	#define INFANTRY_MAX_CURRENT 16380
#endif



/*User Variable*/
extern Motor_C620 wheel_Motor[4];
extern uint8_t gimbal_ctrl[8];
extern bool unlimit_power;
extern uint8_t unlimit_response;
extern float test_speed;



struct UI_Flag{
	uint8_t shift_flag;	//С����
	uint8_t V_flag;		//ˢ��UI
	uint8_t C_flag;		//���٣�������
	uint8_t Q_flag;		//��С��������
	uint8_t B_flag;		//���ص���
};

enum Power_mode{
	power_50W=0U,
	power_55W,
	power_60W,
	power_65W,
	power_70W,
	power_80W,
	power_90W,
	power_100W,
	power_120W
};


class hero_chassis{

public:
		hero_chassis():SourceManage(CAP){};
		myPID LF;		//��ǰ��
		myPID LB;		//�����
		myPID RF;
		myPID RB;		//�����ĸ�����
		float pow_scale;	//�������Ƶı���
		float motor_power;	//�趨��Ŀ�깦��
		float RF_power;			//����ϵͳ��ǰ���ƹ���
		float max_speed;		//����ٶ�ֵ����ֹ�˶�����
		float _speed_target[4]={0};	//�������������ٶ�Ŀ��ֵ
		enum Power_mode _power_mode;
		int16_t x_target=0;
		int16_t y_target=0;
		int16_t r_target=0;
		UI_Flag UI_flag;
		
		void control();
		void speed_control(float x_target,float y_target,float r_target);
		void update();
		void power_switch();
		void Set_target_power();
		void Adjust_speed(float x_scale,float y_scale,float r_scale);
		C_SourceManage_Classdef SourceManage  ;
			
private:
		void power_control();
		void speed_limit(float *speed_target,float max_speed);
		void McNum_resolve(float x_target,float y_target,float r_target);
		void Get_referee_power(Power_mode &power_mode);
		void update_power_data();
		void speed_update(uint8_t *gimbal_ctrl);
		void speed_mode(Power_mode power_mode);
		int unlimit_set(bool flag,Power_mode &power_mode);
		void UI_flag_update(uint8_t total_flag);
};


/*User Function*/

float compare(float input,float min, float max);
float My_ABS(float x);






extern hero_chassis Hero_chassis;

#endif
#endif /*__HERO_CHASSIS_H */