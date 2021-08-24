#include "Chassis_control.h"
#include "pitchyaw.h"
#include "DR16_task.h"
#include "Remote_Keyboard.h"
#include "arm_math.h"


#define power_50W 50
#define power_55W 55
#define power_60W 60
#define power_65W 65
#define power_70W 70
#define power_80W 80
#define power_90W 90
#define power_100W 100
#define power_120W 120



/*User Variable*/
int16_t yaw_encoder;		//yaw�������ֵ��������ֵ�޷���ȡ����can�ж������ȡ��
uint8_t Dial_switch=0;	//��Ӧ���ر�־λ��ɨ����Ӧ����ʱΪ1
uint8_t shoot_flag=0;	//�����־λ��������һ�������ñ�־λΪ1��ͨ��Referee��ȡ�����ж�
float CAP_Voltage=0;
uint8_t	Reset_flag=0;




/**
* @brief  ����Ҫ���͸����̵�����,�����ݴ����msg_pack
* @param  x_target��y_target��r_target�����̵����������ٶ�	 msg_pack:�ϲ�CAN���ݰ�
* @return msg_pack�������������е����ݰ�
*/
CAN_COB C_Chassis::communication(CAN_COB &msg_pack,int16_t x_target,int16_t y_target,int16_t r_target){
	msg_pack.ID=0x111;
	Merge_flag(&UI_flag);
	msg_pack.Data[0]=x_target>>8;
	msg_pack.Data[1]=x_target&0x00FF;
	msg_pack.Data[2]=y_target>>8;
	msg_pack.Data[3]=y_target&0x00FF;
	msg_pack.Data[4]=r_target>>8;
	msg_pack.Data[5]=r_target&0x00FF;
	msg_pack.Data[6]=Upslope;	
	msg_pack.Data[7]=UI_flag.total_flag;
	return msg_pack;
}


/**
 * @brief  �����б�־λ�û�����ϲ���һ��uint8_t
 * @param  UI_flag
 * @return none
 */
void C_Chassis::Merge_flag(UI_Flag* UI_flag){
	UI_flag->total_flag=(uint8_t)(Reset_flag<<5)+(uint8_t)(UI_flag->Shift_flag<<4)+(uint8_t)(UI_flag->Ctrl_flag<<3)+
	(uint8_t)(UI_flag->C_flag<<2)+(uint8_t)(UI_flag->Q_flag<<1)+(uint8_t)(UI_flag->B_flag);
}



/**
 * @brief  ���̸��溯��
 * @param  none
 * @return none
 */
void C_Chassis::chassis_follow(){
	/*С����״̬�˶�����*/
	speed_y=arm_cos_f32(Yaw_Motor[0].getAngle()/180*pi)*y_target+arm_sin_f32(Yaw_Motor[0].getAngle()/180*pi)*x_target;
	speed_x=arm_sin_f32(Yaw_Motor[0].getAngle()/180*pi)*y_target-arm_cos_f32(Yaw_Motor[0].getAngle()/180*pi)*x_target;

	if(yaw_encoder-Target_encoder>4096)
		yaw_encoder=yaw_encoder-8192;
	else if(yaw_encoder-Target_encoder<-4096)
		yaw_encoder=yaw_encoder+8192;
	
	if(((DR16.GetS1()==DR16_SW_DOWN)&&(DR16.GetS2()!=DR16_SW_DOWN))||(DR16.IsKeyPress(DR16_KEY_SHIFT))){			
		/*С����*/
		speed_r=-1000;
		
	}
	else{
		/*���̸���*/
		chassisYawAngle.Current=yaw_encoder;
		chassisYawAngle.Target=Target_encoder;
		speed_r=chassisYawAngle.Adjust();
		speed_r_mode();
	}
	Board_pack=communication(Board_pack,speed_x,speed_y,speed_r);
}


/**
 * @brief  ��̨��Ҫ�Ĳ���ϵͳ���ݸ���
 * @param  
 * @return none
 */
void C_Chassis::Referee1_Update(uint8_t can_rx_data[]){
	Referee.shooter_id1_42mm_speed_limit=(uint16_t)(can_rx_data[0]<<8|can_rx_data[1]);
	Referee.classis_power_limit=(uint16_t)(can_rx_data[2]<<8|can_rx_data[3]);
	Referee.shooter_id1_42mm_cooling_heat=(uint16_t)(can_rx_data[4]<<8|can_rx_data[5]);
	Referee.shooter_id1_42mm_cooling_limit=(uint16_t)(can_rx_data[6]<<8|can_rx_data[7]);
}



/**
 * @brief  ���ͨ����������
 * @param  CAN_RxMessage
 * @return none
 */
void C_Chassis::Referee2_Update(uint8_t can_rx_data[]){
	Referee.robot_id=can_rx_data[0];
	Referee.SuperCap=can_rx_data[1];
	Dial_switch=can_rx_data[2];
	shoot_flag=can_rx_data[3];
	Referee.Power_Shooter_Output=can_rx_data[4];
	CAP_Voltage=can_rx_data[5]<<8|can_rx_data[6];
	CAP_Voltage/=100;
}



/**
 * @brief  speed_r���޷������ݵ��̵�ǰ���ʾ������̸��������ٶ�
 * @param  none
 * @return none
 */
void C_Chassis::speed_r_mode(){
	switch(Referee.classis_power_limit){
		case power_50W:
			speed_r=compare(3200,speed_r);
		break;
		case power_55W:
			speed_r=compare(3525,speed_r);
		break;
		case power_60W:
			speed_r=compare(3850,speed_r);
		break;
		case power_65W:
			speed_r=compare(4175,speed_r);
		break;
		case power_70W:
			speed_r=compare(5500,speed_r);
		break;
		case power_80W:
			speed_r=compare(5700,speed_r);
		break;
		case power_90W:
			speed_r=compare(6250,speed_r);
		break;
		case power_100W:
			speed_r=compare(6800,speed_r);
		break;
		case power_120W:
			speed_r=compare(7500,speed_r);
		break;
		
		default:
			speed_r=compare(5500,speed_r);		//��������3����׼���׶β���ϵͳ����250W���ʣ�����70W����
			break;

	}
}


/**
 * @brief  ���߱���
 * @param  
 * @return 
 */
void C_Chassis::stop(){
	speed_x=0;
	speed_y=0;
	speed_r=0;
	Board_pack=communication(Board_pack,speed_x,speed_y,speed_r);
}

/**
 * @brief  �Ƚ��޷�����
 * @param  
 * @return 
 */
float C_Chassis::compare(float max_speed, float input){
	if(input>max_speed) return 1000;
	else if(input<-max_speed) return -1000;
	else return input/max_speed*1000;	//��speed_r����һ���������Ϊ���������������ٶȷ�Χ��0-1000
}



