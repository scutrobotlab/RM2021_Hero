

#include "Hero_Gimbal.h"


Gimbal Hero_Gimbal;



/**
 * @brief  更新CAN1电机数据
 * @param  CAN_RxMsg：上层的CAN数据包
 * @return none
 */
void Gimbal::CAN1_MotorUpdate(CAN_COB* CAN_RxMsg){
	if(FriMotor[0].CheckID(CAN_RxMsg->ID)){
		FriMotor[0].update(CAN_RxMsg->Data);	//左摩擦轮
	}
	else if(FriMotor[1].CheckID(CAN_RxMsg->ID)){
		FriMotor[1].update(CAN_RxMsg->Data);	//右摩擦轮
	}
	else if(Third_FriMotor[0].CheckID(CAN_RxMsg->ID)){
		Third_FriMotor[0].update(CAN_RxMsg->Data);	//第三摩擦轮
	}
	else if(Pitch_Motor[0].CheckID(CAN_RxMsg->ID)){
		Pitch_Motor[0].update(CAN_RxMsg->Data);    //pitch电机
	}
}

/**
 * @brief  更新CAN2电机数据
 * @param  CAN_RxMsg：上层的CAN数据包
 * @return none
 */
void Gimbal::CAN2_MotorUpdate(CAN_COB* CAN_RxMsg){
	if(Yaw_Motor[0].CheckID(CAN_RxMsg->ID)){
		Yaw_Motor[0].update(CAN_RxMsg->Data);	//yaw电机
		yaw_encoder=(uint16_t)(CAN_RxMsg->Data[0] << 8 | CAN_RxMsg->Data[1]);
	}
	else if(Dial_Motor[0].CheckID(CAN_RxMsg->ID)){
		Dial_Motor[0].update(CAN_RxMsg->Data);	//拨盘电机
	}
	else if(CAN_RxMsg->ID==0x112){
		Chassis.Referee1_Update(CAN_RxMsg->Data);
	}
	else if(CAN_RxMsg->ID==0x113){
		Chassis.Referee2_Update(CAN_RxMsg->Data);
	}
}

/**
 * @brief  将CAN数据处理成数据包，发送队列
 * @param  none
 * @return none
 */
void Gimbal::Can_pack_process(){
	/*打包CAN1的电机数据包*/
	CAN1_pack=MotorMsgPack<Motor_C620,Motor_C620,Motor_C610,Motor_GM6020>(CAN1_pack,FriMotor[0],FriMotor[1],Third_FriMotor[0],Pitch_Motor[0]);
	/*打包CAN2的电机数据包*/
	CAN2_pack=MotorMsgPack<Motor_GM6020,Motor_C620>(CAN2_pack,Yaw_Motor[0],Dial_Motor[0]);
	/*将数据包发送到队列*/
	xQueueSend(CAN1_TxPort, &CAN1_pack.Low, 0);
	xQueueSend(CAN1_TxPort, &CAN1_pack.High, 0);
	xQueueSend(CAN2_TxPort, &CAN2_pack.High, 0);
	xQueueSend(CAN2_TxPort,	&Chassis.Board_pack,0);
}





/**
 * @brief  初始化，设置云台所有PID
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
	
			/*摩擦轮，拨盘*/
	Shoot.Friction_left.SetPIDParam(14,3,0,8000.0f,16384.0f);
	Shoot.Friction_right.SetPIDParam(14,3,0,8000.0f,16384.0f);
	Shoot.Dial_plate_spe.SetPIDParam(20,2,0,20000.0f,30000.0f);
	Shoot.Dial_plate_ang.SetPIDParam(4,0,0,15000.0f,1500.0f);
	Shoot.Third_Friction.SetPIDParam(10,6,0,20000.0f,30000.0f);
			/*取消积分分离*/
	Shoot.Dial_plate_ang.I_SeparThresh=20000;
	Shoot.Dial_plate_spe.I_SeparThresh=20000;
	Shoot.Friction_left.I_SeparThresh=20000;
	Shoot.Friction_right.I_SeparThresh=20000;
	
	
			/*底盘跟随*/
	Chassis.chassisYawAngle.SetPIDParam(5,0,0,30000,30000);
}

/**
 * @brief  云台控制函数
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
 * @brief  云台各电机数据更新
 * @param  none
 * @return none
 */
void Gimbal::update(){
	Shoot.update();
	Pitchyaw.update();
}




/**
 * @brief  掉线保护
 * @param  none
 * @return none
 */
void Gimbal::stop(){
	Shoot.stop();
	Pitchyaw.stop();
	Chassis.stop();
	Can_pack_process();
}