#ifndef __CHASSIS_CONTROL_H
#define __CHASSIS_CONTROL_H
#ifdef __cplusplus


#include "System_DataPool.h"

extern int16_t yaw_encoder;
extern uint8_t Dial_switch;
extern uint8_t shoot_flag;
extern float CAP_Voltage;
extern uint8_t Reset_flag;

struct _referee
{
	uint8_t robot_id;
	uint16_t shooter_id1_42mm_speed_limit;
	uint16_t classis_power_limit;
	uint16_t shooter_id1_42mm_cooling_heat;
	uint16_t shooter_id1_42mm_cooling_limit;
	uint8_t SuperCap;		//底盘反馈能否超功率标志位，当为0时即使操作手按下加速键也不加速，因为此时电容电压过低
	uint8_t Power_Shooter_Output;//发射24V供电状态，为1时正常供电，为0时断电
};

struct UI_Flag{
	uint8_t Shift_flag;	//小陀螺
	uint8_t Ctrl_flag;		//刷新UI
	uint8_t C_flag;		//加速，超功率
	uint8_t Q_flag;		//打小陀螺自瞄
	uint8_t B_flag;		//开关弹仓
	uint8_t total_flag;	//将所有标志位用或运算合并成一个uint8_t
	bool mouse_R_flag;	//云台用，不要发给底盘
};


class C_Chassis
{
	public:
		C_Chassis(){
			Target_encoder=3482;
		}

		void chassis_follow();
		void Referee1_Update(uint8_t can_rx_data[]);
		void Referee2_Update(uint8_t can_rx_data[]);
		float compare(float max_speed ,float input);
		void speed_r_mode();
		void stop();
			/*设置归中位置，方便一键掉头，90度转吊射*/
		void Set_Target_encoder(float Target){
			Target_encoder=Target;
		}
			/*获取当前归中码盘值*/
		float Get_Target_encoder(){
			return Target_encoder;
		}
		
		
		UI_Flag UI_flag;
		myPID chassisYawAngle;
		_referee Referee;
		float pi=3.1415926;
		int16_t speed_x,speed_y,speed_r;	//坐标变换后的速度值，speed_y表示摄像头前方的方向
		uint8_t Upslope;		//上坡标志位，让底盘超功率上坡
		CAN_COB Board_pack;		//板间通信数据包

	private:
		float Target_encoder;		//电机归中码盘值，电机库该值不可读出，故在此定义
		void Merge_flag(UI_Flag* UI_flag);
		CAN_COB communication(CAN_COB &msg_pack,int16_t x_target,int16_t y_target,int16_t r_target);
	
};

extern C_Chassis Chassis;


#endif

#endif	/*__CHASSIS_CONTROL_H */