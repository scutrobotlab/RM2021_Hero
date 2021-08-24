
/**
 * @brief 用于执行视觉视觉对接相关操作,包括 接收数据 和 返回数据
 * @file  PCvision.cpp
**/

#include "PCvision.h"
#include "System_Datapool.h"
extern UART_HandleTypeDef huart6;


/**
 * @brief  发送给视觉的数据包
 * @param  pitchAngle，yawAngle：云台角度值
		   robotID：机器人颜色
		   maxSpeed：射速
		   isChange：切换目标
		   Mode：0时为正常自瞄、4时为吊射模式、5时为引导模式
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
 * @brief  通过检查视觉发送的尾部帧检查小电脑发送的数据是否正常，若前后两次的尾帧相同，则丢弃该数据包
 * @param  none
 * @return none
 */
void PCvision_Classdef::PC_CheckLink(){
	static uint8_t PC_last_End;
	if(PackFromVisionUnion.PackFromVision.End!=PC_last_End)	//视觉发送的尾帧不等于上一次尾帧
		PC_CheckLinkFlag=true;
	else PC_CheckLinkFlag=false;
	PC_last_End=PackFromVisionUnion.PackFromVision.End;
}
