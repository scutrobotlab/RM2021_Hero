#ifndef __PITCHYAW_H
#define __PITCHYAW_H
#ifdef __cplusplus


#include "System_DataPool.h"

extern	Motor_GM6020  Pitch_Motor[1];
extern	Motor_GM6020  Yaw_Motor[1];
extern  float test_angle;

struct MPU_6050{
	float now_mpu6050;
	float last_mpu6050;
	float total_mpu6050;
};


enum angle_mode{
	yaw_motor=0U,
	yaw_MPU,
	pitch_motor,
	pitch_MPU
};



class pitchyaw{
	public:
		void control();
		void update();
		void Set_zero_angle(float pitch_mid,float yaw_mid);
		void reset_motor();
		void stop();
		void Set_Pit_Target(float pit_ang);
		void Set_Yaw_Target(float yaw_ang);
		void Vision_Pit_Target(float offset);
		void Vision_Yaw_Target(float offset);
		myPID Yaw_spe;
		myPID Yaw_ang;
		myPID Pitch_spe;
		myPID Pitch_ang;
		MPU_6050 yaw_mpu;
		MPU_6050 pitch_mpu;
		float compare(float max,float min, float input);
	private:
		void Get_angle_data(angle_mode mode,Motor_GM6020 *motor);
};



#endif

#endif	/*__ PITCHYAW_H */