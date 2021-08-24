#include "board_com.h"
C_BoardCom c_BoardCom;
extern uint8_t Dial_switch;
void C_BoardCom :: Send(referee_Classdef *Referee)
{
	shoot_singletime(Referee);
	Update(Referee);
	CANx_SendData(&hcan1,0x112,data1,8);
	CANx_SendData(&hcan1,0x113,data2,8);
}

void C_BoardCom :: Update(referee_Classdef *Referee)
{
	data1[0] = Referee->GameRobotState.shooter_id1_42mm_speed_limit>>8;
	data1[1] = Referee->GameRobotState.shooter_id1_42mm_speed_limit&0x00FF;
	data1[2] = Referee->GameRobotState.classis_power_limit>>8;
	data1[3] = Referee->GameRobotState.classis_power_limit&0x00FF;
	data1[4] = Referee->PowerHeatData.shooter_id1_42mm_cooling_heat>>8;
	data1[5] = Referee->PowerHeatData.shooter_id1_42mm_cooling_heat&0x00FF;
	data1[6] = Referee->GameRobotState.shooter_id1_42mm_cooling_limit>>8;
	data1[7] = Referee->GameRobotState.shooter_id1_42mm_cooling_limit&0x00FF;
	
	data2[0] = Referee->GameRobotState.robot_id;
	data2[1] = unlimit_response;
	data2[2] = Dial_switch;
	data2[3] = shoot_flag;
	Dial_switch=0;
	shoot_flag=0;
}




/**
 * @brief  判断射速是否变化来判断是否发射一发
 * @param  none
 * @return none
 */
void C_BoardCom::shoot_singletime(referee_Classdef *Referee){
	if(last_shoot!=Referee->ShootData.bullet_speed){	//射速变化，说明此时打出一发，给云台发送已发射一发的标志位
		shoot_flag=1;
	}
	last_shoot=Referee->ShootData.bullet_speed;
}


