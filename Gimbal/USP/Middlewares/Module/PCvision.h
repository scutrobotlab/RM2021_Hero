#ifndef __PCvision_H__
#define __PCvision_H__
#include <SRML.h>
/**************************************************************************
* @brief	�Ӿ���������Э��һ�µ����ݰ�
* @note		Flag: 		��װ�װ� -- 37 
						��װ�װ� -- 72 
			shoot_mode: ��ֹ��� -- 64 
						������� -- 76 
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
* @brief	ͨ�������ڴ潫�Ӿ��������������Զ�ͬ����PackFromVision
* @note		ʹ��UsartData�����Ӿ������ݣ�PackFromVision�ж�Ӧ�Ĳ��������޸�
**************************************************************************/
typedef union 
{
	uint8_t UsartData[11];
	PackFromVisionDef PackFromVision;
}PackFromVisionUnionDef;

/**************************************************************************
* @brief	���ظ��Ӿ���Э��һ�µ����ݰ�
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
	uint8_t isChange;	// �л�Ŀ�� -- 1    	���л�Ŀ�� -- 0 (Ĭ��)
	uint8_t mode;		// ���� -- 0 (Ĭ��) 	���� -- 4	
	uint8_t end;	
}PackToVision_Def;
#pragma pack()
/**************************************************************************
* @brief	ͨ�������ڴ潫�����������Զ�ͬ����PackToVision
* @note		ͨ���޸�PackToVision�еĲ������ٽ�UsartData���鷢�ͳ�����
**************************************************************************/
typedef union 
{
	uint8_t UsartData[14];
	PackToVision_Def PackToVision;
}PackToVisionUnionDef;

/**************************************************************************
* @brief	ͨ�������ڴ漰�����޸�float���͵Ĵ�С��
* @note		
**************************************************************************/


class PCvision_Classdef
{
	public:
		void SendGimbleStatus(float pitchAngle,float yawAngle,uint8_t robotID,uint8_t maxSpeed,uint8_t isChange,uint8_t Mode);
		void PC_CheckLink();
	
		PackFromVisionUnionDef PackFromVisionUnion;
		PackToVisionUnionDef PackToVisionUnion;
		bool PC_CheckLinkFlag=false;		//���С�����Ƿ�����

};


#endif

