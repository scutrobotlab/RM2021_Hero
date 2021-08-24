#include "Remote_Keyboard.h"
#include "Hero_Gimbal.h"

int16_t x_target=0;
int16_t y_target=0;
bool switch_flag=true;	//防止感应开关在一次发射中多次触发的标志位
bool ChangeTarget=false;//视觉切换自瞄目标
uint8_t temp_current_flag = 0;
struct Indicator_State indicator_State;
					/*****************************遥控器控制*****************************/
/**
 * @brief  遥控器控制底盘
 * @param  none
 * @return none
 */
void Remote_Chassial_Ctrl(){
	x_target=-DR16.Get_LX_Norm()*1000;
	y_target=DR16.Get_LY_Norm()*1000;
	if((DR16.GetS2()==DR16_SW_UP)&&(Hero_Gimbal.Chassis.Referee.SuperCap)){
		Hero_Gimbal.Chassis.UI_flag.C_flag=1;
	}
	else {
		Hero_Gimbal.Chassis.UI_flag.C_flag=0;
	}
}


/**
 * @brief  遥控器控制云台
 * @param  none 
 * @return none
 */
void Remote_Gimbal_Ctrl(){
	
	Hero_Gimbal.Pitchyaw.Set_Pit_Target(-0.3*DR16.Get_RY_Norm());
	Hero_Gimbal.Pitchyaw.Set_Yaw_Target(-0.8*DR16.Get_RX_Norm());
};




/**
 * @brief  遥控器控制发射
 * @param  none
 * @return none
 */
void Remote_Shoot_Ctrl(){
	if(shoot_flag){
		switch_flag=true;
	}
	if(DR16.GetS1()==DR16_SW_UP){
		static bool flag=false;
		Hero_Gimbal.Shoot.laser_on();
		Hero_Gimbal.Shoot.Dial_Back();
		switch(Hero_Gimbal.Chassis.Referee.shooter_id1_42mm_speed_limit)//射速限制
		{
			case 10:
				Hero_Gimbal.Shoot.Set_fri_speed(3700);
				break;
			case 16:
				Hero_Gimbal.Shoot.Set_fri_speed(5200);
				break;
			default: 		//检录和3分钟裁判系统会发80米每秒的射速，按16米每秒设置
				Hero_Gimbal.Shoot.Set_fri_speed(5200);
				break;
		}
		if((Hero_Gimbal.Chassis.Referee.shooter_id1_42mm_cooling_limit-
			Hero_Gimbal.Chassis.Referee.shooter_id1_42mm_cooling_heat)>=100){	//判断是否超热量
				if((Dial_switch)&&(!Hero_Gimbal.Shoot.backflag)&&(switch_flag)&&(indicator_State.Dial_back==0)){
					Hero_Gimbal.Shoot.Dial_plate_ang.Target=Switch_Angle-Angle_offset+1140;	//修正目标角度，消除累计误差
					switch_flag=false;
				}
				/*每下拨一次发射一枚子弹*/
				if(DR16.GetS2()==DR16_SW_MID){
					flag=true;
				}
				if(DR16.GetS2()==DR16_SW_DOWN&&flag){
					Hero_Gimbal.Shoot.Set_Dia_angle(1140);
					Hero_Gimbal.Shoot.Set_Third_speed(5000);
					flag=false;
				}	
		}
			/*把摩擦轮当做拨盘用，判断射速停止条件*/
		if(Hero_Gimbal.Shoot.Dial_plate_ang.Target-Hero_Gimbal.Shoot.Dial_plate_ang.Current<300){
			Hero_Gimbal.Shoot.Set_Third_speed(0);
		}
	}
	else{
		Hero_Gimbal.Shoot.laser_off();
		Hero_Gimbal.Shoot.Set_fri_speed(0);
		Hero_Gimbal.Shoot.Set_Third_speed(0);
	}
};





						
							/*****************************键鼠控制*****************************/
/**
 * @brief  键盘控制底盘
 * @param  none
 * @return none
 */
void Keyboard_Chassial_Ctrl(){
	if(DR16.IsKeyPress(DR16_KEY_W))
		y_target=1000;
	else if(DR16.IsKeyPress(DR16_KEY_S))
		y_target=-1000;
	else y_target=0;
	
	if(DR16.IsKeyPress(DR16_KEY_A))
		x_target=1000;
	else if(DR16.IsKeyPress(DR16_KEY_D))
		x_target=-1000;
	else x_target=0;
	
	
	if(((DR16.GetS1()==DR16_SW_DOWN)&&(DR16.GetS2()!=DR16_SW_DOWN))||(DR16.IsKeyPress(DR16_KEY_SHIFT))){
		indicator_State.rotate_state=1;
	}
	else indicator_State.rotate_state=0;
}


/**
 * @brief  超功率加速函数（键盘C键）
 * @param  none
 * @return none
 */
void Keyboard_SuperCap_Ctrl()
{
	if(DR16.IsKeyPress(DR16_KEY_C)){
		Hero_Gimbal.Chassis.UI_flag.C_flag=1;
		indicator_State.Super_CAP=1;	//LED5亮白灯
	}
	else{
		Hero_Gimbal.Chassis.UI_flag.C_flag=0;
	}
	if(!Hero_Gimbal.Chassis.Referee.SuperCap){
		Hero_Gimbal.Chassis.UI_flag.C_flag=0;		//底盘反馈电压过低，不允许超功率加速
	}
}


/**
 * @brief  键盘控制发射（F键开摩擦轮）
 * @param  none
 * @return none
 */
void Keyboard_Shoot_Ctrl(){
	static bool F_flag=false,F_flag_last=false;
	static uint8_t Mouse_L_flag=false, Mouse_L_flag_last=false;
	static bool last_shoot_power=false;	//发射24V供电标志位，为false时不供电
	static bool Shoot_Disable_flag=false;//禁用发射键标志位	
	static uint16_t Shoot_Disable_cnt=0,Mouse_L_cnt=0;	//禁用发射键计数器，0.5s内只允许发射
	static bool Mouse_L_cnt_flag=false;
	static uint16_t LED2_cnt=0;
	if((DR16.IsKeyPress(DR16_KEY_F))&&(!F_flag_last)){
		F_flag=1;
	}
	F_flag_last=DR16.IsKeyPress(DR16_KEY_F);
	


	/*发射一发shoot_flag为1*/
	if(shoot_flag){
		switch_flag=true;
		Shoot_Disable_flag=true;
	}
	
	/*发射禁用位生效，0.5s后才可发射下一发*/
	if(Shoot_Disable_flag){
		Shoot_Disable_cnt++;
		if((Shoot_Disable_cnt>=500)&&(Hero_Gimbal.Chassis.Referee.shooter_id1_42mm_cooling_limit==100)){		//Shoot_Disable_cnt为500时解除发射禁用
			Shoot_Disable_cnt=0;
			Shoot_Disable_flag=false;
		}
		else if((Shoot_Disable_cnt>=50)&&(Hero_Gimbal.Chassis.Referee.shooter_id1_42mm_cooling_limit!=100)){
			Shoot_Disable_cnt=0;
			Shoot_Disable_flag=false;
		}
	}
	
	/*一级英雄鼠标左键禁用0.8s，防止操作手瞬间猛按鼠标导致超热量，Mouse_L_cnt_flag为true期间禁用鼠标左键*/
	if(Mouse_L_cnt_flag){
		Mouse_L_cnt++;
		
		/*热量为100时为一级英雄，为防止连发，鼠标禁用0.8s*/
		if(Hero_Gimbal.Chassis.Referee.shooter_id1_42mm_cooling_limit==100){
			if(Mouse_L_cnt>=800){
				Mouse_L_cnt=0;
				Mouse_L_cnt_flag=false;
			}
		}
		/*非一级英雄鼠标禁用0.15s*/
		else{
			if(Mouse_L_cnt>=150){
				Mouse_L_cnt=0;
				Mouse_L_cnt_flag=false;
			}
		}
	}

	
		/*按F打开摩擦轮*/
	if(F_flag){		
		if(indicator_State.shoot_state!=2){
			LED2_cnt++;
			if(LED2_cnt>=200){
				LED2_cnt=0;
				indicator_State.shoot_state=2;//灭灯
			}
		}
		if(Hero_Gimbal.Shoot.backflag){
		indicator_State.shoot_state=1;//红灯
		}
		Hero_Gimbal.Shoot.laser_on();
		
			/*回拨检测*/
		Hero_Gimbal.Shoot.Dial_Back();
			
		
			/*射速判断*/
		switch(Hero_Gimbal.Chassis.Referee.shooter_id1_42mm_speed_limit){	
			case 10:
				Hero_Gimbal.Shoot.Set_fri_speed(3700);
				break;
			case 16:
				Hero_Gimbal.Shoot.Set_fri_speed(5200);
				break;
			default: 
				Hero_Gimbal.Shoot.Set_fri_speed(5200);
				break;
		}
		
			/*鼠标上升沿判断，长按鼠标和短按鼠标均只算一次发射*/
		temp_current_flag = DR16.IsKeyPress(DR16_MOUSE_L);
		if((temp_current_flag)&&(!Mouse_L_flag_last)&&(!Mouse_L_cnt_flag)){
				Mouse_L_flag=true;
		}
		else Mouse_L_flag=false;
		Mouse_L_flag_last=temp_current_flag;
		
		
		
			/*感应开关修正角度*/
		if((Dial_switch)&&(!Hero_Gimbal.Shoot.backflag)&&(switch_flag)){
			Hero_Gimbal.Shoot.Dial_plate_ang.Target=Switch_Angle-Angle_offset+1140;	
			switch_flag=false;	//防止感应开关在一次发射中多次触发的标志位
		}
		
			/*满足条件按下鼠标发射一发*/
		else if((Mouse_L_flag)&&(Hero_Gimbal.Shoot.backflag==false)&&
				(Hero_Gimbal.Chassis.Referee.Power_Shooter_Output)&&(!Shoot_Disable_flag)){			
			if((Hero_Gimbal.Chassis.Referee.shooter_id1_42mm_cooling_limit-
				Hero_Gimbal.Chassis.Referee.shooter_id1_42mm_cooling_heat)>=100){	//判断是否超热量
				Hero_Gimbal.Shoot.Set_Dia_angle(1140);
				Hero_Gimbal.Shoot.Set_Third_speed(5000);	//第三摩擦轮当拨盘用
				Mouse_L_cnt_flag=true;
				indicator_State.shoot_state=0;	//正常发射
			}
		}
			/*把第三摩擦轮当做拨盘用，判断射速停止条件*/
		if(Hero_Gimbal.Shoot.Dial_plate_ang.Target-Hero_Gimbal.Shoot.Dial_plate_ang.Current<50){
			Hero_Gimbal.Shoot.Set_Third_speed(0);
		}
		

		if(!Hero_Gimbal.Chassis.Referee.Power_Shooter_Output){		//发射电断电
			Hero_Gimbal.Shoot.Dial_angle_reset();
			indicator_State.shoot_state=3;
		}
	}
	
		/*按G关闭摩擦轮*/
	if(DR16.IsKeyPress(DR16_KEY_G)){		
		Hero_Gimbal.Shoot.laser_off();
		Hero_Gimbal.Shoot.Set_fri_speed(0);
		Hero_Gimbal.Shoot.Set_Third_speed(0);
		F_flag=0;
	}
	last_shoot_power=Hero_Gimbal.Chassis.Referee.Power_Shooter_Output;
}

/**
 * @brief  键鼠控制云台
 * @param  none
 * @return none
 */
void Keyboard_PitchYaw_Ctrl(){
	static bool R_flag=false,R_flag_last=false;
	static bool X_flag=false,X_flag_last=false;
	if((DR16.IsKeyPress(DR16_KEY_R))&&(!R_flag_last)){
		R_flag=1;
	}
	R_flag_last=DR16.IsKeyPress(DR16_KEY_R);
		/*一键调头*/
	if(R_flag){
		if(Hero_Gimbal.Chassis.Get_Target_encoder()==3482){	//LED3灯珠为黄色，头向弹仓（灯条）
			Hero_Gimbal.Chassis.Set_Target_encoder(7578);
			Hero_Gimbal.Pitchyaw.Set_Yaw_Target(180);
			indicator_State.shoot90_state=2;
		}
		else if(Hero_Gimbal.Chassis.Get_Target_encoder()==7578){//LED3灯珠不亮
			Hero_Gimbal.Chassis.Set_Target_encoder(3482);
			Hero_Gimbal.Pitchyaw.Set_Yaw_Target(180);	
			indicator_State.shoot90_state=0;
		}
		R_flag=0;
	}	
	
	if((DR16.IsKeyPress(DR16_KEY_X))&&(!X_flag_last)){
		X_flag=1;
	}
	X_flag_last=DR16.IsKeyPress(DR16_KEY_X);
		/*90度吊射*/
	if(X_flag){
		if(Hero_Gimbal.Chassis.Get_Target_encoder()!=5530){
			Hero_Gimbal.Chassis.Set_Target_encoder(5530);
			Hero_Gimbal.Pitchyaw.Set_Yaw_Target(-90);
			indicator_State.shoot90_state=1;	//处于90度吊射模式，LED3灯珠蓝色
		}
		else{
			Hero_Gimbal.Chassis.Set_Target_encoder(3482);
			Hero_Gimbal.Pitchyaw.Set_Yaw_Target(90);
			indicator_State.shoot90_state=0;	//恢复正常状态
		}
		X_flag=0;
	}

	if(DR16.IsKeyPress(DR16_MOUSE_R)){	//视觉自瞄
		if(Hero_Gimbal.PCvision.PackFromVisionUnion.PackFromVision.flag!=72){	//视觉没有找到装甲板，允许鼠标控制
			Hero_Gimbal.Pitchyaw.Set_Pit_Target(1.0*DR16.Get_MouseY_Norm());
			Hero_Gimbal.Pitchyaw.Set_Yaw_Target(-0.8*DR16.Get_MouseX_Norm());
		}
		Hero_Gimbal.Chassis.UI_flag.mouse_R_flag=1;
	}
	else {//鼠标控制，正常模式
		Hero_Gimbal.Pitchyaw.Set_Pit_Target(1.0*DR16.Get_MouseY_Norm());
		Hero_Gimbal.Pitchyaw.Set_Yaw_Target(-0.8*DR16.Get_MouseX_Norm());
		Hero_Gimbal.Chassis.UI_flag.mouse_R_flag=0;
	}
}




/**
 * @brief  英雄上坡模式
 * @param  none
 * @return none
 */
void Keyboard_Upslope(){
	if(DR16.IsKeyPress(DR16_KEY_V)){
		Hero_Gimbal.Chassis.Upslope=true;
		indicator_State.Super_CAP=2;	//LED5紫灯
	}
	else {
		Hero_Gimbal.Chassis.Upslope=false;
	}
}





void Keyboard_flag_Ctrl(){
	static bool last_B_flag=false;
	
	if(DR16.IsKeyPress(DR16_KEY_CTRL)){
		Hero_Gimbal.Chassis.UI_flag.Ctrl_flag=1;
	}
	else Hero_Gimbal.Chassis.UI_flag.Ctrl_flag=0;
	if((DR16.IsKeyPress(DR16_KEY_SHIFT))){
		Hero_Gimbal.Chassis.UI_flag.Shift_flag=1;
	}
	else Hero_Gimbal.Chassis.UI_flag.Shift_flag=0;
	
	if((DR16.IsKeyPress(DR16_KEY_B))&&(!last_B_flag)){
		Hero_Gimbal.Chassis.UI_flag.B_flag=!Hero_Gimbal.Chassis.UI_flag.B_flag;
		Hero_Gimbal.Shoot.Set_Dia_angle(-2280);
		indicator_State.Dial_back=1;
	}
	Hero_Gimbal.Shoot.Back_error();
	last_B_flag=DR16.IsKeyPress(DR16_KEY_B);
}



/**
 * @brief  切换自瞄目标
 * @param  none
 * @return none
 */
void Keyboard_ChangeTarget(){
	if(DR16.IsKeyPress(DR16_KEY_Q)){
		ChangeTarget=true;
	}
	else ChangeTarget=false;
}



/**
 * @brief  主动复位按键
 * @param  none
 * @return none
 */
void Keyboard_Reset(){
	static uint8_t Reset_cnt=0;
	if((DR16.IsKeyPress(DR16_KEY_G))&&(DR16.IsKeyPress(DR16_KEY_CTRL))){
		/*关闭云台所有电机，给底盘发0*/
		Hero_Gimbal.stop();
		Reset_flag=1;

	}
	if(Reset_flag){
		Reset_cnt++;
	}
	if(Reset_cnt==5){
		Reset_cnt=0;
		__set_FAULTMASK(1);//关闭所有中断
		NVIC_SystemReset();//复位函数
	}
}