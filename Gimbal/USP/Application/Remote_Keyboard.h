#ifndef __REMOTE_KEYBOARD_H
#define __REMOTE_KEYBOARD_H

#ifdef __cplusplus

#include "System_Datapool.h"

struct Indicator_State{
	uint8_t shoot_state=2;	//0时正常发射，1时回拨，2时灭灯，3时发射断电
	bool rotate_state;
	uint8_t shoot90_state;	//0时正常，灭灯。1时90度转头，吊射蓝灯。2时180度转头，黄灯
	uint8_t Super_CAP;		//0时灭灯，常规功率。1时超速，白灯。2时上坡，紫灯。
	uint8_t Dial_back;		//主动回拨，0时灭灯。1时成功主动回拨，绿灯。2时回拨异常，顶到感应开关，红灯
};


extern int16_t x_target;
extern int16_t y_target;
extern struct Indicator_State indicator_State;
extern bool ChangeTarget;
/*****************************遥控器控制*****************************/
void Remote_Chassial_Ctrl();
void Remote_Gimbal_Ctrl();
void Remote_Shoot_Ctrl();
/*****************************键鼠控制*****************************/
void Keyboard_Chassial_Ctrl();
void Keyboard_SuperCap_Ctrl();
void Keyboard_Shoot_Ctrl();
void Keyboard_PitchYaw_Ctrl();
void Keyboard_flag_Ctrl();
void Keyboard_Upslope();
void Keyboard_ChangeTarget();
void Keyboard_Reset();

#endif
#endif /*__REMOTE_KEYBOARD_H */