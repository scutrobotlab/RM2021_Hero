

#include "Hero_Gimbal.h"


Gimbal Hero_Gimbal;



/**
 * @brief  ����CAN1�������
 * @param  CAN_RxMsg���ϲ��CAN���ݰ�
 * @return none
 */
void Gimbal::CAN1_MotorUpdate(CAN_COB* CAN_RxMsg){
	if(FriMotor[0].CheckID(CAN_RxMsg->ID)){
		FriMotor[0].update(CAN_RxMsg->Data);	//��Ħ����
	}
	else if(FriMotor[1].CheckID(CAN_RxMsg->ID)){
		FriMotor[1].update(CAN_RxMsg->Data);	//��Ħ����
	}
	else if(Third_FriMotor[0].CheckID(CAN_RxMsg->ID)){
		Third_FriMotor[0].update(CAN_RxMsg->Data);	//����Ħ����
	}
	else if(Pitch_Motor[0].CheckID(CAN_RxMsg->ID)){
		Pitch_Motor[0].update(CAN_RxMsg->Data);    //pitch���
	}
}

/**
 * @brief  ����CAN2�������
 * @param  CAN_RxMsg���ϲ��CAN���ݰ�
 * @return none
 */
void Gimbal::CAN2_MotorUpdate(CAN_COB* CAN_RxMsg){
	if(Yaw_Motor[0].CheckID(CAN_RxMsg->ID)){
		Yaw_Motor[0].update(CAN_RxMsg->Data);	//yaw���
		yaw_encoder=(uint16_t)(CAN_RxMsg->Data[0] << 8 | CAN_RxMsg->Data[1]);
	}
	else if(Dial_Motor[0].CheckID(CAN_RxMsg->ID)){
		Dial_Motor[0].update(CAN_RxMsg->Data);	//���̵��
	}
	else if(CAN_RxMsg->ID==0x112){
		Chassis.Referee1_Update(CAN_RxMsg->Data);
	}
	else if(CAN_RxMsg->ID==0x113){
		Chassis.Referee2_Update(CAN_RxMsg->Data);
	}
}

/**
 * @brief  ��CAN���ݴ�������ݰ������Ͷ���
 * @param  none
 * @return none
 */
void Gimbal::Can_pack_process(){
	/*���CAN1�ĵ�����ݰ�*/
	CAN1_pack=MotorMsgPack<Motor_C620,Motor_C620,Motor_C610,Motor_GM6020>(CAN1_pack,FriMotor[0],FriMotor[1],Third_FriMotor[0],Pitch_Motor[0]);
	/*���CAN2�ĵ�����ݰ�*/
	CAN2_pack=MotorMsgPack<Motor_GM6020,Motor_C620>(CAN2_pack,Yaw_Motor[0],Dial_Motor[0]);
	/*�����ݰ����͵�����*/
	xQueueSend(CAN1_TxPort, &CAN1_pack.Low, 0);
	xQueueSend(CAN1_TxPort, &CAN1_pack.High, 0);
	xQueueSend(CAN2_TxPort, &CAN2_pack.High, 0);
	xQueueSend(CAN2_TxPort,	&Chassis.Board_pack,0);
}





/**
 * @brief  ��ʼ����������̨����PID
 * @param  none
 * @return none
 */
void Gimbal::Gimbal_Init(){
			/*Pitch,Yaw*/
	Pitchyaw.Pitch_spe.SetPIDParam(20,130,0,25000,30000);	
	Pitchyaw.Pitch_ang.SetPIDParam(150,0,0,15000,30000);	
	Pitchyaw.Yaw_spe.SetPIDParam(6000,10000,0,20000,30000);		
	Pitchyaw.Yaw_ang.SetPIDParam(1.4,0,0,15000,20000);
	Pitchyaw.Set_zero_angle(7188,3482);
	
			/*Ħ���֣�����*/
	Shoot.Friction_left.SetPIDParam(14,3,0,8000.0f,16384.0f);
	Shoot.Friction_right.SetPIDParam(14,3,0,8000.0f,16384.0f);
	Shoot.Dial_plate_spe.SetPIDParam(20,2,0,20000.0f,30000.0f);
	Shoot.Dial_plate_ang.SetPIDParam(4,0,0,15000.0f,1500.0f);
	Shoot.Third_Friction.SetPIDParam(10,6,0,20000.0f,30000.0f);
			/*ȡ�����ַ���*/
	Shoot.Dial_plate_ang.I_SeparThresh=20000;
	Shoot.Dial_plate_spe.I_SeparThresh=20000;
	Shoot.Friction_left.I_SeparThresh=20000;
	Shoot.Friction_right.I_SeparThresh=20000;
	
	
			/*���̸���*/
	Chassis.chassisYawAngle.SetPIDParam(5,0,0,30000,30000);
}

/**
 * @brief  ��̨���ƺ���
 * @param  none
 * @return none
 */
void Gimbal::control(){
	Shoot.control();
	Pitchyaw.control();
	Chassis.chassis_follow();
	Can_pack_process();
}



/**
 * @brief  ��̨��������ݸ���
 * @param  none
 * @return none
 */
void Gimbal::update(){
	Shoot.update();
	Pitchyaw.update();
}




/**
 * @brief  ���߱���
 * @param  none
 * @return none
 */
void Gimbal::stop(){
	Shoot.stop();
	Pitchyaw.stop();
	Chassis.stop();
	Can_pack_process();
}