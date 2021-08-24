#ifndef __PCvision_H__
#define __PCvision_H__
#include <SRML.h>
/**************************************************************************
* @brief	视觉传过来的协商一致的数据包
* @note		Flag: 		有装甲板 -- 37 
						无装甲板 -- 72 
			shoot_mode: 禁止射击 -- 64 
						允许射击 -- 76 
**************************************************************************/

#pragma pack(1)
typedef struct 
{
	uint8_t flag;
	uint8_t shootMode;
	float yawData;
	float pitchData;
	uint8_t End;
}PackFromVisionDef;
#pragma pack()
/**************************************************************************
* @brief	通过共用内存将视觉传过来的数据自动同步到PackFromVision
* @note		使用UsartData接收视觉的数据，PackFromVision中对应的参数将被修改
**************************************************************************/
typedef union 
{
	uint8_t UsartData[11];
	PackFromVisionDef PackFromVision;
}PackFromVisionUnionDef;

/**************************************************************************
* @brief	返回给视觉的协商一致的数据包
* @note		Null
**************************************************************************/
#pragma pack(1)
typedef struct
{
	uint8_t head;
	float	pitchAngle;
	float	yawAngle;
	uint8_t color;
	uint8_t bulletSpeed;
	uint8_t isChange;	// 切换目标 -- 1    	不切换目标 -- 0 (默认)
	uint8_t mode;		// 自瞄 -- 0 (默认) 	吊射 -- 4	
	uint8_t end;	
}PackToVision_Def;
#pragma pack()
/**************************************************************************
* @brief	通过共用内存将反馈的数据自动同步到PackToVision
* @note		通过修改PackToVision中的参数，再将UsartData数组发送出即可
**************************************************************************/
typedef union 
{
	uint8_t UsartData[14];
	PackToVision_Def PackToVision;
}PackToVisionUnionDef;

/**************************************************************************
* @brief	通过共用内存及函数修改float类型的大小端
* @note		
**************************************************************************/


class PCvision_Classdef
{
	public:
		void SendGimbleStatus(float pitchAngle,float yawAngle,uint8_t robotID,uint8_t maxSpeed,uint8_t isChange,uint8_t Mode);
		void PC_CheckLink();
	
		PackFromVisionUnionDef PackFromVisionUnion;
		PackToVisionUnionDef PackToVisionUnion;
		bool PC_CheckLinkFlag=false;		//检查小电脑是否在线

};


#endif

