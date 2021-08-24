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

#define SUPPLY_BY_BAT 1 /* 电池供电，限制电流上限，防止过流保护 */
#define INFANTRY_MAX_SPEED 9000		//超过这个速度，运动变形
#if SUPPLY_BY_BAT
	#define INFANTRY_MAX_CURRENT 7000	//输出的最大速度
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
	uint8_t shift_flag;	//小陀螺
	uint8_t V_flag;		//刷新UI
	uint8_t C_flag;		//加速，超功率
	uint8_t Q_flag;		//打小陀螺自瞄
	uint8_t B_flag;		//开关弹仓
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
		myPID LF;		//左前轮
		myPID LB;		//左后轮
		myPID RF;
		myPID RB;		//底盘四个麦轮
		float pow_scale;	//功率限制的比率
		float motor_power;	//设定的目标功率
		float RF_power;			//裁判系统当前限制功率
		float max_speed;		//最大速度值，防止运动变形
		float _speed_target[4]={0};	//解算后的四麦轮速度目标值
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