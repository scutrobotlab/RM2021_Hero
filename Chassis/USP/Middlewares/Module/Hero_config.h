#ifndef __HERO_CONFIG_H
#define __HERO_CONFIG_H
#ifdef __cplusplus

#include "Hero_chassis.h"
#include "move_task.h"
#include "referee.h"
#include "board_com.h"


extern referee_Classdef Referee;
extern myPID chassis_power_ctrl;
extern myPID capCtrl;
extern PowerCtrl_ClassDef PowerCtrl;



void Hero_Config_Init();
float MotorLimitController(float current,float target);
float CapChargeController(const float current, const float target);


#endif
#endif /*__HERO_CONFIG_H */