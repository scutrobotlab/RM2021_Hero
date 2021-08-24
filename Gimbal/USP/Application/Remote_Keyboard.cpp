#include "Remote_Keyboard.h"
#include "Hero_Gimbal.h"

int16_t x_target=0;
int16_t y_target=0;
bool switch_flag=true;	//��ֹ��Ӧ������һ�η����ж�δ����ı�־λ
bool ChangeTarget=false;//�Ӿ��л�����Ŀ��
uint8_t temp_current_flag = 0;
struct Indicator_State indicator_State;
					/*****************************ң��������*****************************/
/**
 * @brief  ң�������Ƶ���
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
 * @brief  ң����������̨
 * @param  none 
 * @return none
 */
void Remote_Gimbal_Ctrl(){
	
	Hero_Gimbal.Pitchyaw.Set_Pit_Target(-0.3*DR16.Get_RY_Norm());
	Hero_Gimbal.Pitchyaw.Set_Yaw_Target(-0.8*DR16.Get_RX_Norm());
};




/**
 * @brief  ң�������Ʒ���
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
		switch(Hero_Gimbal.Chassis.Referee.shooter_id1_42mm_speed_limit)//��������
		{
			case 10:
				Hero_Gimbal.Shoot.Set_fri_speed(3700);
				break;
			case 16:
				Hero_Gimbal.Shoot.Set_fri_speed(5200);
				break;
			default: 		//��¼��3���Ӳ���ϵͳ�ᷢ80��ÿ������٣���16��ÿ������
				Hero_Gimbal.Shoot.Set_fri_speed(5200);
				break;
		}
		if((Hero_Gimbal.Chassis.Referee.shooter_id1_42mm_cooling_limit-
			Hero_Gimbal.Chassis.Referee.shooter_id1_42mm_cooling_heat)>=100){	//�ж��Ƿ�����
				if((Dial_switch)&&(!Hero_Gimbal.Shoot.backflag)&&(switch_flag)&&(indicator_State.Dial_back==0)){
					Hero_Gimbal.Shoot.Dial_plate_ang.Target=Switch_Angle-Angle_offset+1140;	//����Ŀ��Ƕȣ������ۼ����
					switch_flag=false;
				}
				/*ÿ�²�һ�η���һö�ӵ�*/
				if(DR16.GetS2()==DR16_SW_MID){
					flag=true;
				}
				if(DR16.GetS2()==DR16_SW_DOWN&&flag){
					Hero_Gimbal.Shoot.Set_Dia_angle(1140);
					Hero_Gimbal.Shoot.Set_Third_speed(5000);
					flag=false;
				}	
		}
			/*��Ħ���ֵ��������ã��ж�����ֹͣ����*/
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





						
							/*****************************�������*****************************/
/**
 * @brief  ���̿��Ƶ���
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
 * @brief  �����ʼ��ٺ���������C����
 * @param  none
 * @return none
 */
void Keyboard_SuperCap_Ctrl()
{
	if(DR16.IsKeyPress(DR16_KEY_C)){
		Hero_Gimbal.Chassis.UI_flag.C_flag=1;
		indicator_State.Super_CAP=1;	//LED5���׵�
	}
	else{
		Hero_Gimbal.Chassis.UI_flag.C_flag=0;
	}
	if(!Hero_Gimbal.Chassis.Referee.SuperCap){
		Hero_Gimbal.Chassis.UI_flag.C_flag=0;		//���̷�����ѹ���ͣ����������ʼ���
	}
}


/**
 * @brief  ���̿��Ʒ��䣨F����Ħ���֣�
 * @param  none
 * @return none
 */
void Keyboard_Shoot_Ctrl(){
	static bool F_flag=false,F_flag_last=false;
	static uint8_t Mouse_L_flag=false, Mouse_L_flag_last=false;
	static bool last_shoot_power=false;	//����24V�����־λ��Ϊfalseʱ������
	static bool Shoot_Disable_flag=false;//���÷������־λ	
	static uint16_t Shoot_Disable_cnt=0,Mouse_L_cnt=0;	//���÷������������0.5s��ֻ������
	static bool Mouse_L_cnt_flag=false;
	static uint16_t LED2_cnt=0;
	if((DR16.IsKeyPress(DR16_KEY_F))&&(!F_flag_last)){
		F_flag=1;
	}
	F_flag_last=DR16.IsKeyPress(DR16_KEY_F);
	


	/*����һ��shoot_flagΪ1*/
	if(shoot_flag){
		switch_flag=true;
		Shoot_Disable_flag=true;
	}
	
	/*�������λ��Ч��0.5s��ſɷ�����һ��*/
	if(Shoot_Disable_flag){
		Shoot_Disable_cnt++;
		if((Shoot_Disable_cnt>=500)&&(Hero_Gimbal.Chassis.Referee.shooter_id1_42mm_cooling_limit==100)){		//Shoot_Disable_cntΪ500ʱ����������
			Shoot_Disable_cnt=0;
			Shoot_Disable_flag=false;
		}
		else if((Shoot_Disable_cnt>=50)&&(Hero_Gimbal.Chassis.Referee.shooter_id1_42mm_cooling_limit!=100)){
			Shoot_Disable_cnt=0;
			Shoot_Disable_flag=false;
		}
	}
	
	/*һ��Ӣ������������0.8s����ֹ������˲���Ͱ���굼�³�������Mouse_L_cnt_flagΪtrue�ڼ����������*/
	if(Mouse_L_cnt_flag){
		Mouse_L_cnt++;
		
		/*����Ϊ100ʱΪһ��Ӣ�ۣ�Ϊ��ֹ������������0.8s*/
		if(Hero_Gimbal.Chassis.Referee.shooter_id1_42mm_cooling_limit==100){
			if(Mouse_L_cnt>=800){
				Mouse_L_cnt=0;
				Mouse_L_cnt_flag=false;
			}
		}
		/*��һ��Ӣ��������0.15s*/
		else{
			if(Mouse_L_cnt>=150){
				Mouse_L_cnt=0;
				Mouse_L_cnt_flag=false;
			}
		}
	}

	
		/*��F��Ħ����*/
	if(F_flag){		
		if(indicator_State.shoot_state!=2){
			LED2_cnt++;
			if(LED2_cnt>=200){
				LED2_cnt=0;
				indicator_State.shoot_state=2;//���
			}
		}
		if(Hero_Gimbal.Shoot.backflag){
		indicator_State.shoot_state=1;//���
		}
		Hero_Gimbal.Shoot.laser_on();
		
			/*�ز����*/
		Hero_Gimbal.Shoot.Dial_Back();
			
		
			/*�����ж�*/
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
		
			/*����������жϣ��������Ͷ̰�����ֻ��һ�η���*/
		temp_current_flag = DR16.IsKeyPress(DR16_MOUSE_L);
		if((temp_current_flag)&&(!Mouse_L_flag_last)&&(!Mouse_L_cnt_flag)){
				Mouse_L_flag=true;
		}
		else Mouse_L_flag=false;
		Mouse_L_flag_last=temp_current_flag;
		
		
		
			/*��Ӧ���������Ƕ�*/
		if((Dial_switch)&&(!Hero_Gimbal.Shoot.backflag)&&(switch_flag)){
			Hero_Gimbal.Shoot.Dial_plate_ang.Target=Switch_Angle-Angle_offset+1140;	
			switch_flag=false;	//��ֹ��Ӧ������һ�η����ж�δ����ı�־λ
		}
		
			/*��������������귢��һ��*/
		else if((Mouse_L_flag)&&(Hero_Gimbal.Shoot.backflag==false)&&
				(Hero_Gimbal.Chassis.Referee.Power_Shooter_Output)&&(!Shoot_Disable_flag)){			
			if((Hero_Gimbal.Chassis.Referee.shooter_id1_42mm_cooling_limit-
				Hero_Gimbal.Chassis.Referee.shooter_id1_42mm_cooling_heat)>=100){	//�ж��Ƿ�����
				Hero_Gimbal.Shoot.Set_Dia_angle(1140);
				Hero_Gimbal.Shoot.Set_Third_speed(5000);	//����Ħ���ֵ�������
				Mouse_L_cnt_flag=true;
				indicator_State.shoot_state=0;	//��������
			}
		}
			/*�ѵ���Ħ���ֵ��������ã��ж�����ֹͣ����*/
		if(Hero_Gimbal.Shoot.Dial_plate_ang.Target-Hero_Gimbal.Shoot.Dial_plate_ang.Current<50){
			Hero_Gimbal.Shoot.Set_Third_speed(0);
		}
		

		if(!Hero_Gimbal.Chassis.Referee.Power_Shooter_Output){		//�����ϵ�
			Hero_Gimbal.Shoot.Dial_angle_reset();
			indicator_State.shoot_state=3;
		}
	}
	
		/*��G�ر�Ħ����*/
	if(DR16.IsKeyPress(DR16_KEY_G)){		
		Hero_Gimbal.Shoot.laser_off();
		Hero_Gimbal.Shoot.Set_fri_speed(0);
		Hero_Gimbal.Shoot.Set_Third_speed(0);
		F_flag=0;
	}
	last_shoot_power=Hero_Gimbal.Chassis.Referee.Power_Shooter_Output;
}

/**
 * @brief  ���������̨
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
		/*һ����ͷ*/
	if(R_flag){
		if(Hero_Gimbal.Chassis.Get_Target_encoder()==3482){	//LED3����Ϊ��ɫ��ͷ�򵯲֣�������
			Hero_Gimbal.Chassis.Set_Target_encoder(7578);
			Hero_Gimbal.Pitchyaw.Set_Yaw_Target(180);
			indicator_State.shoot90_state=2;
		}
		else if(Hero_Gimbal.Chassis.Get_Target_encoder()==7578){//LED3���鲻��
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
		/*90�ȵ���*/
	if(X_flag){
		if(Hero_Gimbal.Chassis.Get_Target_encoder()!=5530){
			Hero_Gimbal.Chassis.Set_Target_encoder(5530);
			Hero_Gimbal.Pitchyaw.Set_Yaw_Target(-90);
			indicator_State.shoot90_state=1;	//����90�ȵ���ģʽ��LED3������ɫ
		}
		else{
			Hero_Gimbal.Chassis.Set_Target_encoder(3482);
			Hero_Gimbal.Pitchyaw.Set_Yaw_Target(90);
			indicator_State.shoot90_state=0;	//�ָ�����״̬
		}
		X_flag=0;
	}

	if(DR16.IsKeyPress(DR16_MOUSE_R)){	//�Ӿ�����
		if(Hero_Gimbal.PCvision.PackFromVisionUnion.PackFromVision.flag!=72){	//�Ӿ�û���ҵ�װ�װ壬����������
			Hero_Gimbal.Pitchyaw.Set_Pit_Target(1.0*DR16.Get_MouseY_Norm());
			Hero_Gimbal.Pitchyaw.Set_Yaw_Target(-0.8*DR16.Get_MouseX_Norm());
		}
		Hero_Gimbal.Chassis.UI_flag.mouse_R_flag=1;
	}
	else {//�����ƣ�����ģʽ
		Hero_Gimbal.Pitchyaw.Set_Pit_Target(1.0*DR16.Get_MouseY_Norm());
		Hero_Gimbal.Pitchyaw.Set_Yaw_Target(-0.8*DR16.Get_MouseX_Norm());
		Hero_Gimbal.Chassis.UI_flag.mouse_R_flag=0;
	}
}




/**
 * @brief  Ӣ������ģʽ
 * @param  none
 * @return none
 */
void Keyboard_Upslope(){
	if(DR16.IsKeyPress(DR16_KEY_V)){
		Hero_Gimbal.Chassis.Upslope=true;
		indicator_State.Super_CAP=2;	//LED5�ϵ�
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
 * @brief  �л�����Ŀ��
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
 * @brief  ������λ����
 * @param  none
 * @return none
 */
void Keyboard_Reset(){
	static uint8_t Reset_cnt=0;
	if((DR16.IsKeyPress(DR16_KEY_G))&&(DR16.IsKeyPress(DR16_KEY_CTRL))){
		/*�ر���̨���е���������̷�0*/
		Hero_Gimbal.stop();
		Reset_flag=1;

	}
	if(Reset_flag){
		Reset_cnt++;
	}
	if(Reset_cnt==5){
		Reset_cnt=0;
		__set_FAULTMASK(1);//�ر������ж�
		NVIC_SystemReset();//��λ����
	}
}