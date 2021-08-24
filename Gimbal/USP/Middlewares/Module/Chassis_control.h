#ifndef __CHASSIS_CONTROL_H
#define __CHASSIS_CONTROL_H
#ifdef __cplusplus


#include "System_DataPool.h"

extern int16_t yaw_encoder;
extern uint8_t Dial_switch;
extern uint8_t shoot_flag;
extern float CAP_Voltage;
extern uint8_t Reset_flag;

struct _referee
{
	uint8_t robot_id;
	uint16_t shooter_id1_42mm_speed_limit;
	uint16_t classis_power_limit;
	uint16_t shooter_id1_42mm_cooling_heat;
	uint16_t shooter_id1_42mm_cooling_limit;
	uint8_t SuperCap;		//���̷����ܷ񳬹��ʱ�־λ����Ϊ0ʱ��ʹ�����ְ��¼��ټ�Ҳ�����٣���Ϊ��ʱ���ݵ�ѹ����
	uint8_t Power_Shooter_Output;//����24V����״̬��Ϊ1ʱ�������磬Ϊ0ʱ�ϵ�
};

struct UI_Flag{
	uint8_t Shift_flag;	//С����
	uint8_t Ctrl_flag;		//ˢ��UI
	uint8_t C_flag;		//���٣�������
	uint8_t Q_flag;		//��С��������
	uint8_t B_flag;		//���ص���
	uint8_t total_flag;	//�����б�־λ�û�����ϲ���һ��uint8_t
	bool mouse_R_flag;	//��̨�ã���Ҫ��������
};


class C_Chassis
{
	public:
		C_Chassis(){
			Target_encoder=3482;
		}

		void chassis_follow();
		void Referee1_Update(uint8_t can_rx_data[]);
		void Referee2_Update(uint8_t can_rx_data[]);
		float compare(float max_speed ,float input);
		void speed_r_mode();
		void stop();
			/*���ù���λ�ã�����һ����ͷ��90��ת����*/
		void Set_Target_encoder(float Target){
			Target_encoder=Target;
		}
			/*��ȡ��ǰ��������ֵ*/
		float Get_Target_encoder(){
			return Target_encoder;
		}
		
		
		UI_Flag UI_flag;
		myPID chassisYawAngle;
		_referee Referee;
		float pi=3.1415926;
		int16_t speed_x,speed_y,speed_r;	//����任����ٶ�ֵ��speed_y��ʾ����ͷǰ���ķ���
		uint8_t Upslope;		//���±�־λ���õ��̳���������
		CAN_COB Board_pack;		//���ͨ�����ݰ�

	private:
		float Target_encoder;		//�����������ֵ��������ֵ���ɶ��������ڴ˶���
		void Merge_flag(UI_Flag* UI_flag);
		CAN_COB communication(CAN_COB &msg_pack,int16_t x_target,int16_t y_target,int16_t r_target);
	
};

extern C_Chassis Chassis;


#endif

#endif	/*__CHASSIS_CONTROL_H */