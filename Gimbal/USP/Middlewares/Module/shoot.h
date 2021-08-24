#ifndef __SHOOT_H
#define __SHOOT_H

#ifdef __cplusplus

#include <stdint.h>
#include <main.h>
#include "System_DataPool.h"
#include "Chassis_control.h"
#include <SRML.h>

extern Motor_C620 Dial_Motor[1];
extern Motor_C620 FriMotor[2];
extern Motor_C610 Third_FriMotor[1];
extern float  Switch_Angle;
extern float Angle_offset;


class shoot{
	public:

		void laser_on();
		void laser_off();
		void control();
		void update();
		void stop();
		void Set_fri_speed(float speed);
		void Set_Third_speed(float speed);
		void Set_Dia_angle(float angle);
		void Dial_angle_reset();
		myPID Friction_left;
		myPID Friction_right;
		myPID Dial_plate_spe;
		myPID Dial_plate_ang;
		myPID Third_Friction;
		void Dial_Back();
		bool Back_error();
		bool backflag;
};


	 
	 
	 
	 
	  
	  
	
#endif
#endif /*__ SHOOT_H */