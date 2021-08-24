/**
  ******************************************************************************
  * Copyright (c) 2019 - ~, SCUT-RobotLab Development Team
  * @file    Indicator.cpp
  * @author  Lingzi_Xie 1357657340@qq.com
  * @brief   State Indicaotr in Robomaster 2019
  * @date    2021-04-30
  * @version 1.0
  * @par Change Log��
  * <table>
  * <tr><th>Date        	<th>Version  <th>Author    		<th>Description
  * <tr><td>2021-05-28   	<td> 1.0     <td>Lingzi_Xie    	<td> Create Module
  * </table>
  *
  ==============================================================================
                            How to use this Module  
  ==============================================================================
    @note 
		-# 2021���״ָ̬ʾ�Ʋ���PWM���������Կ���ռ����̨����һ��С4pin UART�ڣ�������ΪTIM��PWM���
		
			-# Cube MX���ú�TIM��PWM���ģʽ��
			��1��ʵ�����ͨ��ʱ��������ȡԤ��Ƶϵ��Ϊ0��ARR��ֵΪ104��ʹPWMƵ��Ϊ800KHz��
			��2��ʹ��TIM��DMA���ͣ�ע��DMA���䷽������Ϊ�ڴ� -> ���裬�����ݿ������Ϊ�֣�
			
			-# �½�Indicator_Classdefʵ������ʹ�ù��캯����ʼ��������TIM�Ͷ�Ӧͨ��DMA�ľ����
			
			-# ����Change_Singal_RGB()����ָ��LED�����ȡ���ɫ��ָ��LED���Ϊ1~6
			  ʾ��
			  		indicator.Change_Singal_RGB(LED_1|LED_2, 0xff, 0, 0xff, 150);
			
			-# ������LED����ɫ�󣬵���Update()��ָʾ��״̬���и��£�
			
		-# ״ָ̬ʾ�Ƶ����Ž��ߣ�
			G����
			V��5V��Դ
			U��PWM�ź�

  ******************************************************************************
  * @attention
  * 
  * if you had modified this file, please make sure your code does not have many 
  * bugs, update the version Number, write dowm your name and the date, the most
  * important is make sure the users will have clear and definite understanding 
  * through your new brief.
  *
  * <h2><center>&copy; Copyright (c) 2019 - ~, SCUT-RobotLab Development Team.
  * All rights reserved.</center></h2>
  ******************************************************************************
*/

#include "Indicator.h"


/**
 * @brief   ָ��RGB����ɫ������
 * @param  	led_num��LED���
			r��g��b:RGB������ɫ��Ϊ0x00ʱ��0xffʱ��
			strength����ɫ���ȣ�Ϊ0x00ʱ������0xffʱ�
 * @return 	none
 */
void Indicator_Classdef::Change_Singal_RGB(uint8_t led_num, uint8_t r, uint8_t g, uint8_t b, uint8_t strength)
{
	uint32_t rgb_code;
	uint8_t i, num_mask = 0x01;
	
	rgb_code = RGB2BIN(r, g, b, strength);
  
	for(i = 1; i <= 6; i++)
	{
		if(led_num & num_mask)
			BIN2CODE(i, rgb_code);
		led_num = led_num >> 1;
	}
}

/* ��������LED��״̬ */
void Indicator_Classdef::Update()
{
	Indicator_LED.ON(buffer);
}

/* �ر�״ָ̬ʾ�� */
void Indicator_Classdef::Reset()
{
	uint16_t count_i, count_j;
	
	for(count_i = 0;count_i < LED_NUM;count_i++)									//����������Ϊ�͵�ƽ��Ӧ����
		for(count_j = 0;count_j < RGB_NUM;count_j++)
			buffer[count_i][count_j] = LED_LOW;
	
	Indicator_LED.ON(buffer);
}

/* ���ɵ���LED���������� */
uint32_t Indicator_Classdef::RGB2BIN(uint8_t r, uint8_t g, uint8_t b, uint8_t strength)
{
	if(strength >= 1 && strength <= 255)
	{
		r = r / strength;
		b = b / strength;
		g = g / strength;
		return ((g << 16) | (r << 8) | b);		
	}
	
	else return 0;
}

/* ���ɵ���LED��PWM CCRֵ����д����������Ӧλ�� */
void Indicator_Classdef::BIN2CODE(uint8_t led_num, uint32_t rgb_bin)
{
	uint16_t count_i;
	
	for(count_i = 0;count_i < RGB_NUM;count_i++)
	{
		buffer[led_num - 1][count_i] = ((rgb_bin << count_i) & 0x800000) ? LED_HIGH : LED_LOW;
	}
}

/* ����״ָ̬ʾ */
void Indicator_Classdef::Cap_Energy_Indicator(float current_volt, float max_volt, float high_volt, float low_volt, uint8_t led_num, uint8_t strenght)
{
	static uint8_t volt_proportion, cap_status, indicator_tick;

	//������
	if(max_volt < high_volt || high_volt < low_volt)
		return;
	
	//����״̬���
	if(current_volt < low_volt)
		cap_status = CAP_CHARGE;
	if(current_volt > high_volt)
		cap_status = CAP_READY;
	
	//�������
	if((current_volt*current_volt - low_volt*low_volt)/(max_volt*max_volt - low_volt*low_volt)<0.3 && cap_status != CAP_CHARGE)
	{
		//����С��30%ʱ���Ʊ��
		Change_Singal_RGB(led_num, 0xff, 0xff, 0, strenght);
	}
	else if(cap_status == CAP_CHARGE)
	{
		//���ڳ��ʱ����ɫ��˸
		if(indicator_tick < 50)
			Change_Singal_RGB(led_num, 0xff, 0, 0, strenght);
		else
			Change_Singal_RGB(led_num, 0, 0, 0, strenght);
		
		indicator_tick++;
		indicator_tick %= 100;
	}
	else
	{
		//���ݵ�ѹ��ʱ���Ʊ���
		Change_Singal_RGB(led_num, 0, 0xff, 0, strenght);
	}
}
