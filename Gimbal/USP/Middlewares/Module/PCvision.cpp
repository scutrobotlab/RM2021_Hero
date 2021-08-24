
/**
 * @brief ����ִ���Ӿ��Ӿ��Խ���ز���,���� �������� �� ��������
 * @file  PCvision.cpp
**/

#include "PCvision.h"
#include "System_Datapool.h"
extern UART_HandleTypeDef huart6;


/**
 * @brief  ���͸��Ӿ������ݰ�
 * @param  pitchAngle��yawAngle����̨�Ƕ�ֵ
		   robotID����������ɫ
		   maxSpeed������
		   isChange���л�Ŀ��
		   Mode��0ʱΪ�������顢4ʱΪ����ģʽ��5ʱΪ����ģʽ
 * @return none
 */
void PCvision_Classdef::SendGimbleStatus(float pitchAngle,float yawAngle,uint8_t robotID,uint8_t maxSpeed,uint8_t isChange,uint8_t Mode)
{	
		switch(maxSpeed){
			case 10:
				break;
			case 16:
				break;
			default: maxSpeed=16;
				break;
		}
		PackToVisionUnion.PackToVision.pitchAngle = pitchAngle;
		PackToVisionUnion.PackToVision.yawAngle = yawAngle;
		PackToVisionUnion.PackToVision.color = (robotID>100)?1:0;
		PackToVisionUnion.PackToVision.bulletSpeed =maxSpeed;
		PackToVisionUnion.PackToVision.isChange = isChange;	
		PackToVisionUnion.PackToVision.mode=Mode;

	
		PackToVisionUnion.PackToVision.head = 0x44;
		PackToVisionUnion.PackToVision.end = 0x55;
		HAL_UART_Transmit_DMA(&huart6, PackToVisionUnion.UsartData, 14);
}



/**
 * @brief  ͨ������Ӿ����͵�β��֡���С���Է��͵������Ƿ���������ǰ�����ε�β֡��ͬ�����������ݰ�
 * @param  none
 * @return none
 */
void PCvision_Classdef::PC_CheckLink(){
	static uint8_t PC_last_End;
	if(PackFromVisionUnion.PackFromVision.End!=PC_last_End)	//�Ӿ����͵�β֡��������һ��β֡
		PC_CheckLinkFlag=true;
	else PC_CheckLinkFlag=false;
	PC_last_End=PackFromVisionUnion.PackFromVision.End;
}
