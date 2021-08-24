/**
  ******************************************************************************
  * Copyright (c) 2021 - ~, SCUT-RobotLab Development Team
  * @file    Indicator.h
  * @author  Lingzi_Xie 1357657340@qq.com
  * @brief   Code for New Indicator board in Robomaster2021.
  ******************************************************************************
  * @attention
  * 
  * if you had modified this file, please make sure your code does not have many 
  * bugs, update the version Number, write dowm your name and the date, the most
  * important is make sure the users will have clear and definite understanding 
  * through your new brief.
  *
  * <h2><center>&copy; Copyright (c) 2021 - ~, SCUT-RobotLab Development Team.
  * All rights reserved.</center></h2>
  ******************************************************************************
  */ 

#ifndef __INDICATOR_H
#define __INDICATOR_H

#ifdef __cplusplus

/* Includes ------------------------------------------------------------------*/  
#include "led.h"

/* define --------------------------------------------------------------------*/
#define LED_1											0x01
#define LED_2											0x02
#define LED_3											0x04			
#define LED_4											0x08
#define LED_5											0x10
#define LED_6											0x20

/* Private define ------------------------------------------------------------*/
#define CAP_READY 1																				//����׼����
#define CAP_CHARGE 0																			//���ݳ����
#define LED_NUM 6																				//״ָ̬ʾ�Ƶĵ�����Ŀ

/* Exported types ------------------------------------------------------------*/
class Indicator_Classdef{
	public:
	
	Indicator_Classdef(TIM_HandleTypeDef *htimer, DMA_HandleTypeDef *_hdma, uint32_t _channel):Indicator_LED(htimer, _hdma, _channel){};
	
	void Change_Singal_RGB(uint8_t led_num, uint8_t r, uint8_t g, uint8_t b, uint8_t strength);		//ָ������RGB����ɫ������
	void Update();																					//��������LED��״̬
	void Reset();																					//�ر�״ָ̬ʾ��
	
	void Cap_Energy_Indicator(float current_volt, float max_volt, float high_volt, float low_volt, uint8_t led_num, uint8_t strenght);		//����״ָ̬ʾ
	
	private:
	
	color cols[LED_NUM];																			//RGB��ɫ�ṹ��
	LED<LED_NUM> Indicator_LED;																		//���ò��������Ĺ��촴���������
	uint32_t buffer[LED_NUM][RGB_NUM];																//����PWM CCRֵ��������������������ź�
	
	uint32_t RGB2BIN(uint8_t r, uint8_t g, uint8_t b, uint8_t strength);							//���ɵ���LED����������
	void BIN2CODE(uint8_t led_num, uint32_t rgb_bin);												//���ɵ���LED��PWM CCRֵ����д����������Ӧλ��
};
#endif

#endif
