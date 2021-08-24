#ifndef __REMOTE_KEYBOARD_H
#define __REMOTE_KEYBOARD_H

#ifdef __cplusplus

#include "System_Datapool.h"

struct Indicator_State{
	uint8_t shoot_state=2;	//0ʱ�������䣬1ʱ�ز���2ʱ��ƣ�3ʱ����ϵ�
	bool rotate_state;
	uint8_t shoot90_state;	//0ʱ��������ơ�1ʱ90��תͷ���������ơ�2ʱ180��תͷ���Ƶ�
	uint8_t Super_CAP;		//0ʱ��ƣ����湦�ʡ�1ʱ���٣��׵ơ�2ʱ���£��ϵơ�
	uint8_t Dial_back;		//�����ز���0ʱ��ơ�1ʱ�ɹ������ز����̵ơ�2ʱ�ز��쳣��������Ӧ���أ����
};


extern int16_t x_target;
extern int16_t y_target;
extern struct Indicator_State indicator_State;
extern bool ChangeTarget;
/*****************************ң��������*****************************/
void Remote_Chassial_Ctrl();
void Remote_Gimbal_Ctrl();
void Remote_Shoot_Ctrl();
/*****************************�������*****************************/
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