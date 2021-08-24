#include "pitchyaw.h"



Motor_GM6020  Pitch_Motor[1]={Motor_GM6020(1)};
Motor_GM6020  Yaw_Motor[1]={Motor_GM6020(2)};



/**
 * @brief  获取码盘或陀螺仪数据
 * @param  mode:yaw_motor、yaw_MPU、pitch_motor、pitch_MPU依次为yaw、pitch的码盘值和陀螺仪值
 * @return None
 */
void pitchyaw::Get_angle_data(angle_mode mode,Motor_GM6020 *motor){
	static int16_t pitch_round_cnt;
	static int16_t yaw_round_cnt;
	
	switch(mode){
		case yaw_motor:{
			Yaw_ang.Current=motor->getAngle();
			Yaw_spe.Current=motor->getSpeed();
		}
		break;
		
		case pitch_motor:{
			Pitch_ang.Current=motor->getAngle();
//			Pitch_spe.Current=motor->getSpeed();
			Pitch_spe.Current=-MPUData.gy;
		}
		break;
		
		case yaw_MPU:{
			yaw_mpu.now_mpu6050 = MPUData.yaw;
			Yaw_spe.Current = MPUData.gz/16.4*60/360;		//换算成rpm，和6020的数据量纲一致

			if(yaw_mpu.now_mpu6050 - yaw_mpu.last_mpu6050 > 180.0f)
			{
				yaw_round_cnt --;
			}
			else if(yaw_mpu.now_mpu6050 - yaw_mpu.last_mpu6050 < -180.0f)
			{
				yaw_round_cnt ++;
			}
			yaw_mpu.last_mpu6050 = yaw_mpu.now_mpu6050;
			yaw_mpu.total_mpu6050 = yaw_mpu.now_mpu6050 + yaw_round_cnt * 360;
			Yaw_ang.Current=yaw_mpu.total_mpu6050;
		}
		break;

		case pitch_MPU:{
			pitch_mpu.now_mpu6050 = MPUData.pitch;
			Pitch_spe.Current = MPUData.gy;

			if(pitch_mpu.now_mpu6050 - pitch_mpu.last_mpu6050 > 180.0f)
			{
				pitch_round_cnt --;
			}
			else if(pitch_mpu.now_mpu6050 - pitch_mpu.last_mpu6050 < -180.0f)
			{
				pitch_round_cnt ++;
			}
			pitch_mpu.last_mpu6050 = pitch_mpu.now_mpu6050;
			pitch_mpu.total_mpu6050 = pitch_mpu.now_mpu6050 + pitch_round_cnt * 360;
			Pitch_ang.Current=pitch_mpu.total_mpu6050;
		}
		break;
	}
}


/**
 * @brief  设置pitch、yaw轴中值为角度0度
 * @param  pitch_mid、yaw_mid：pitch、yaw轴的中值对应的码盘值
 * @return none
 */
void pitchyaw::Set_zero_angle(float pitch_mid,float yaw_mid){
	Pitch_Motor[0].setEncoderOffset(pitch_mid);
	Yaw_Motor[0].setEncoderOffset(yaw_mid);
}


/**
 * @brief  上电时电机归中
 * @param  none
 * @return none
 */
void pitchyaw::reset_motor(){
	Pitch_ang.Target=0;
	Yaw_ang.Target=yaw_mpu.total_mpu6050;		//yaw在当前角度归中，用底盘跟随来归中
}



/**
 * @brief  比较限幅函数
 * @param  
 * @return 
 */
float pitchyaw::compare(float max,float min, float input){
	if(input>max) return max;
	else if(input<min) return min;
	else return input;
}



/**
 * @brief  更新pitch,yaw数据
 * @param  none
 * @return none
 */
void pitchyaw::update(){
	Get_angle_data(pitch_motor,Pitch_Motor);
	Get_angle_data(yaw_MPU,Yaw_Motor);


}


float test_angle=0.0;
void pitchyaw::control(){

	Pitch_spe.Target=Pitch_ang.Adjust();
	Yaw_spe.Target=Yaw_ang.Adjust();
	
	Pitch_Motor[0].Out=Pitch_spe.Adjust();
	Yaw_Motor[0].Out=-Yaw_spe.Adjust();

//	MotorMsgSend(&hcan1,Pitch_Motor);
//	MotorMsgSend(&hcan2,Yaw_Motor);
}



/**
 * @brief  设置pitch，yaw轴目标角度值
 * @param  pit_ang、yaw_ang
 * @return none
 */
void pitchyaw::Set_Pit_Target(float pit_ang){
	Pitch_ang.Target+=pit_ang;
	Pitch_ang.Target=compare(22,-40,Pitch_ang.Target);
}
void pitchyaw::Set_Yaw_Target(float yaw_ang){
	Yaw_ang.Target+=yaw_ang;
}



/**
 * @brief  自瞄时视觉矫正云台
 * @param  offset:视觉发送过来的当前值与目标的偏差，注意用当前角度加该值
 * @return none
 */
void pitchyaw::Vision_Pit_Target(float offset){
	Pitch_ang.Target=compare(70,-100,Pitch_Motor[0].getAngle()+offset);
}
void pitchyaw::Vision_Yaw_Target(float offset){
	Yaw_ang.Target=yaw_mpu.total_mpu6050+offset;
}




/**
 * @brief  掉线保护
 * @param  
 * @return 
 */
void pitchyaw::stop(){
	Yaw_ang.Target=yaw_mpu.total_mpu6050;	
	Pitch_Motor[0].Out=0;
	Yaw_Motor[0].Out=0;
//	MotorMsgSend(&hcan1,Pitch_Motor);
//	MotorMsgSend(&hcan2,Yaw_Motor);
}