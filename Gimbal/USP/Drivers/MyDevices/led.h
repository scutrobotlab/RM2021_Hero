/**
  ******************************************************************************
  * Copyright (c) 2021 - ~, SCUT-RobotLab Development Team
  * @file    led.h
  * @author  LJY 2250017028@qq.com
  * @brief   Code for LED.
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

#ifndef __LED_H
#define __LED_H	 

#ifdef __cplusplus

/* Includes ------------------------------------------------------------------*/  
#include "main.h"
#include <algorithm>

/* Private define ------------------------------------------------------------*/
#define RGB_NUM 	24		//һ��RGB����Ҫ��PWM������Ŀ
#define RESET_NUM	250		//��λ�ƴ���Ҫ��PWM������Ŀ
#define LED_LOW 	34		//0�źŵ�ռ�ձ�
#define LED_HIGH 	68		//1�źŵ�ռ�ձ�									

/* Private include -----------------------------------------------------------*/

/* Private macros ------------------------------------------------------------*/			

/* Private type --------------------------------------------------------------*/

/* ��ɫ��ṹ�� */
typedef struct RGB
{	
	uint8_t red;
	uint8_t green;
	uint8_t blue;
}color;

/* Exported macros -----------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
template<uint16_t NUM>
class LED
{
public:
	uint32_t Temp_Buffer[NUM][RGB_NUM];							//��32λ�Ķ�ʱ��ʹ�õĻ�����
	uint32_t Reset_RGB[RESET_NUM];								//250������ʹ����������ź�>280us���ܰ������ƴ�����

	LED(TIM_HandleTypeDef *htimer, DMA_HandleTypeDef *_hdma, uint32_t _channel)
	: ledPwmTimer(htimer), hdma(_hdma), Channel(_channel){}
		
	/**@brief ����
	*@param[in] data ��ɫ����
	*@return 0 success
			 1 fail
	*/
	uint8_t ON(uint32_t data[NUM][RGB_NUM])
	{	
		uint16_t i, j;
		uint8_t res;
		
		/* ���ڶ�ʱ��2�ļĴ���Ϊ32λ,���Էֿ����� */
//		if(ledPwmTimer->Instance == TIM2)
//		{
//			for(i = 0; i < NUM; i ++)
//			{
//				for(j = 0; j < RGB_NUM; j ++) Temp_Buffer[i][j] = data[i][j];
//			}
//			res = Set((uint32_t *)Temp_Buffer, NUM * RGB_NUM);
//		}
//		else 
//		{
//			res = Set((uint32_t *)data, NUM * RGB_NUM);
//		}
		
		res = Set((uint32_t *)data, NUM * RGB_NUM);		
		
		/* ���յƴ�ʱ��ÿ��д�����ݺ��跢�͸�λ�ź� */
		Reset();
		return res;
	}

private:
	TIM_HandleTypeDef* ledPwmTimer;									//�������PWM��TIM���
	DMA_HandleTypeDef* hdma;										//TIM��Ӧ��DMA
	uint32_t Channel;												//TIM���ͨ��
	
	/**@brief ������ɫ����
		*@param void
		*@return 0 success
				 1 fail
	*/
	uint8_t Set(uint32_t* data, uint32_t num)
	{
		if(!Data_Transmit(data, num)) return 0;
		else return 1;	
	}

	/**@brief ������������
		*@param void
		*@return 0 success
				 1 fail
	*/
	uint8_t Reset(void)
	{
		if(!Data_Transmit(Reset_RGB,RESET_NUM)) return 0;
		else return 1;
	}

	/**@brief ���ݴ���
		*@param[in]	buffer 	���黺��
		*@param[in] num	 ������Ŀ
		*@return 0 success
				 1 fail
	*/
	uint8_t Data_Transmit(uint32_t* buffer, uint32_t num)
	{	
		/* ��ʼ�������� */
		while(HAL_TIM_PWM_Start_DMA(ledPwmTimer, Channel, buffer, num) != HAL_OK);
		
		/* �ȴ�������� */
		while(HAL_DMA_GetState(hdma) != HAL_DMA_STATE_READY);
		
		/* ��ͨ����DMAҪ�ر� */
		while(HAL_TIM_PWM_Stop_DMA(ledPwmTimer, Channel) != HAL_OK);
		
		return 0;
	}	
};
/* Exported variables --------------------------------------------------------*/

/* Exported function declarations --------------------------------------------*/
#endif	
#endif
