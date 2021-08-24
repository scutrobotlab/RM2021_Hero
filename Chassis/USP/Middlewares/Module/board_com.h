#ifndef BOARD_COM_H
#define BOARD_COM_H
#ifdef __cplusplus

#include "FreeRTOS.h"
#include "PID.h"
#include <main.h>
#include "drv_can.h"
#include "referee.h"
#include "Hero_chassis.h"


extern CAN_HandleTypeDef hcan1;
extern referee_Classdef Referee;

class C_BoardCom
{
	public:
		void Update(referee_Classdef *Referee);
		void Send(referee_Classdef *Referee);
		uint8_t data1[8];
		uint8_t data2[8];
		float last_shoot;	//上一发子弹的射速
		uint8_t shoot_flag=0;	//为1时，发射了一发弹丸
		void shoot_singletime(referee_Classdef *Referee);
};

extern C_BoardCom c_BoardCom;

#endif
#endif /*BOARD_COM_H */