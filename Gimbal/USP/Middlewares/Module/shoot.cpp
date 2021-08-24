#include "shoot.h"
#include "Remote_Keyboard.h"

Motor_C620 Dial_Motor[1]={Motor_C620(7)};
Motor_C620 FriMotor[2]={Motor_C620(1),Motor_C620(2)};
Motor_C610 Third_FriMotor[1]={Motor_C610(3)};
float Switch_Angle=0.0;	//光电开关处角度值
float Angle_offset=1010;




void shoot::laser_on(){
	HAL_GPIO_WritePin(Laser_GPIO_Port,Laser_Pin,GPIO_PIN_SET);
}

void shoot::laser_off(){
	HAL_GPIO_WritePin(Laser_GPIO_Port,Laser_Pin,GPIO_PIN_RESET);
}


/**
 * @brief  更新发射部分数据
 * @param  none
 * @return none
 */
void shoot::update(){
	Friction_left.Current=FriMotor[0].getSpeed();
	Friction_right.Current=FriMotor[1].getSpeed();
	Third_Friction.Current=Third_FriMotor[0].getSpeed();
	Dial_plate_ang.Current=Dial_Motor[0].getAngle();
	Dial_plate_spe.Current=Dial_Motor[0].getSpeed();
}



void shoot::control(){
	FriMotor[0].Out=Friction_left.Adjust();
	FriMotor[1].Out=Friction_right.Adjust();
	Third_FriMotor[0].Out=Third_Friction.Adjust();
	Dial_plate_spe.Target=Dial_plate_ang.Adjust();
	Dial_Motor[0].Out=Dial_plate_spe.Adjust();

}


void shoot::Dial_Back()
{
	static uint32_t Back_cnt1=0,Back_cnt2=0,Back_count=0;
	if(Dial_plate_ang.Target-Dial_plate_ang.Current>590)
	{		
		Back_cnt1++;
		if(Back_cnt1>=500)
		{
			Dial_plate_ang.Target=Dial_plate_ang.Target-1710;//回退一格
			backflag=true;
			Back_cnt1=0;
		}
	}
	else
	{
		Back_cnt1=0;
	}
	
	
	if(Dial_plate_spe.Out>20000){
		Back_cnt2++;
		if(Back_cnt2>=600){
			Dial_plate_ang.Target=Dial_plate_ang.Target-1710;//回退一格
			backflag=true;
			Back_cnt2=0;
		}
	}
	else{
		Back_cnt2=0;
	}
	if(backflag==true)
	{
		Back_count++;
		if(Back_count>=500)
		{
		Back_count=0;
		backflag=false;
		}
	}
}



/**
 * @brief  操作手主动回拨失败
 * @param  
 * @return true:回拨失败  false:回拨成功
 */
bool shoot::Back_error(){
	static uint16_t error_count=0;
	if(indicator_State.Dial_back==1){
		if(Dial_plate_spe.Out<-25000){
			error_count++;
		}
		else error_count=0;
		if(error_count==400){
			error_count=0;
			Dial_plate_ang.Target=Dial_plate_ang.Current+20;
			indicator_State.Dial_back=2;
			return true;
		}
	}
	return false;
}




/**
 * @brief  关闭摩擦轮，拨盘
 * @param  none
 * @return none
 */
void shoot::stop(){
	Set_fri_speed(0);
	Set_Third_speed(0);
	Dial_plate_ang.Target=Dial_plate_ang.Current;
	FriMotor[0].Out=0;
	FriMotor[1].Out=0;
	Third_FriMotor[0].Out=0;
	Dial_Motor[0].Out=0;
	laser_off();
//	MotorMsgSend(&hcan2,Dial_Motor);
//	MotorMsgSend(&hcan1,FriMotor);
//	MotorMsgSend(&hcan1,Third_FriMotor);
}


/**
 * @brief  设置摩擦轮射速
 * @param  speed:摩擦轮射速目标值
 * @return none
 */
void shoot::Set_fri_speed(float speed){
	Friction_left.Target=speed;
	Friction_right.Target=-speed;
}

/**
 * @brief  设置第三摩擦轮射速
 * @param  speed:第三摩擦轮射速目标值
 * @return none
 */
void shoot::Set_Third_speed(float speed){
	Third_Friction.Target=speed;
}



/**
 * @brief  设置拨盘角度
 * @param  angle：转动目标角度
 * @return none
 */
void shoot::Set_Dia_angle(float angle){
	Dial_plate_ang.Target+=angle;
}


/**
 * @brief  发射电被断时，拨盘目标值为当前值，防止购买子弹后弹丸被挤出去
 * @param  none
 * @return none
 */
void shoot::Dial_angle_reset(){
	Dial_plate_ang.Target=Dial_plate_ang.Current;
}




