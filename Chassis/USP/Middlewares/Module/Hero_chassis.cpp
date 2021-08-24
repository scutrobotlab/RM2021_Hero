#include "Hero_chassis.h"


/*User Variable*/
hero_chassis Hero_chassis;
Motor_C620 wheel_Motor[4]={Motor_C620(1),Motor_C620(2),Motor_C620(3),Motor_C620(4)};	//LF=1,RF=2,RB=3,LB=4
uint8_t gimbal_ctrl[8]={0};	//��̨��������������Ŀ���ٶ� 
bool unlimit_power=false;	//�����ʱ�־λ���������ְ��¼���C��ʱ�ñ�־λΪtrue
uint8_t unlimit_response=1;	//�����ʻظ�����Ϊ0ʱ��ʹ�����ְ��¼��ټ�Ҳ�����٣���Ϊ��ʱ���ݵ�ѹ����




/**
 * @brief  ����޷�����chassis.cpp��һ��һ����ģ��
 * @param  input������ֵ��min��max���޷���Χ
 * @return �޷�������
 */
float compare(float input,float min, float max){
	if(input>max){
		return max;
	}
	else if(input<min){
		return min;
	}
	else 
		return input;
}


/**
* @brief  ���������������˶��ֱ����ò�ͬ������ٶȣ���Target�Ͽ��ƣ�
* @param  x_scale,y_scale,r_scale�����ʣ���̨����������ֵΪ0~1000���˸ñ���ΪĿ���ٶ�
* @return none
*/
void hero_chassis::Adjust_speed(float x_scale,float y_scale,float r_scale){
	x_target*=x_scale;
	y_target*=y_scale;
	r_target*=r_scale;
}



/**
* @brief  ���������ٶȣ���ֹĳ�������ٶȹ������˶�����
* @param  wheel_Motor:���ֵ��ָ��
* @return none
*/
void hero_chassis::speed_limit(float *speed_target,float max_speed){
	float speed_scale;
	float Temp[4]={0};
	for (int i = 0; i < 4; i++ ){
		Temp[i] = _Chassis_Abs(speed_target[i]);
	}
	_Chassis_BubbleSort(Temp,4);
	if(Temp[3]>max_speed){
		speed_scale=max_speed/Temp[3];
	}
	else 
		speed_scale=1.0;
	
	for(int i=0;i<4;i++){
		speed_target[i]*=speed_scale;
	}
}

float My_ABS(float x){
	if(x>0)
		return x;
	else 
		return -x;
}





/**
* @brief  ������̨���͵��ٶ�ֵ
* @param  *gimbal_ctrl����̨�����ٶȵ�ָ��
* @return none
*/
void hero_chassis::speed_update(uint8_t *gimbal_ctrl){
	x_target=gimbal_ctrl[0]<<8|gimbal_ctrl[1];
	y_target=gimbal_ctrl[2]<<8|gimbal_ctrl[3];
	r_target=gimbal_ctrl[4]<<8|gimbal_ctrl[5];
	unlimit_power=gimbal_ctrl[6];
	UI_flag_update(gimbal_ctrl[7]);
}


/**
 * @brief  ���ֽ���
*  @param  x_target:���ҵ��ٶ�Ϊ����y_target����ǰ���ٶ�Ϊ����r_target����ʱ����תΪ��
 * @return None
 */
void hero_chassis::McNum_resolve(float x_target,float y_target,float r_target ){
	_speed_target[0]=x_target+y_target-r_target;
	_speed_target[1]=x_target-y_target-r_target;
	_speed_target[2]=-x_target+y_target-r_target;
	_speed_target[3]=-x_target-y_target-r_target;
}

/**
 * @brief  ���̿��ƺ���
 * @param  none
 * @return none
 */
void hero_chassis::control(){
	/*���µ�������*/
	update();
	
	/*�����ٶȿ���*/
	speed_control(x_target,y_target,r_target);
	
	/*���ʿ���*/
	power_control();
}

/**
 * @brief  �����˶���������
*  @param  x_target:���ҵ��ٶ�Ϊ����y_target����ǰ���ٶ�Ϊ����r_target����ʱ����תΪ��
 * @return None
 */
void hero_chassis::speed_control(float x_target,float y_target,float r_target ){
	McNum_resolve(x_target,y_target,r_target);
	speed_limit(_speed_target,max_speed);
	
	LF.Target=_speed_target[0];
	RF.Target=_speed_target[1];
	LB.Target=_speed_target[2];
	RB.Target=_speed_target[3];
	
	LF.Current=wheel_Motor[0].getSpeed();
	RF.Current=wheel_Motor[1].getSpeed();
	RB.Current=wheel_Motor[2].getSpeed();
	LB.Current=wheel_Motor[3].getSpeed();
	
	
	wheel_Motor[0].Out=compare(LF.Adjust(),-INFANTRY_MAX_CURRENT,INFANTRY_MAX_CURRENT);
	wheel_Motor[1].Out=compare(RF.Adjust(),-INFANTRY_MAX_CURRENT,INFANTRY_MAX_CURRENT);
	wheel_Motor[2].Out=compare(RB.Adjust(),-INFANTRY_MAX_CURRENT,INFANTRY_MAX_CURRENT);
	wheel_Motor[3].Out=compare(LB.Adjust(),-INFANTRY_MAX_CURRENT,INFANTRY_MAX_CURRENT);
}



/**
 * @brief  ���µ����������
 * @param  none
 * @return none
 */
void hero_chassis::update(){
	speed_update(gimbal_ctrl);
	update_power_data();
}



/**
 * @brief  ������������������̹���
 * @param  None
 * @return None
 */
void hero_chassis::power_switch(){
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_1|GPIO_PIN_2,GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3|GPIO_PIN_4 |GPIO_PIN_5, GPIO_PIN_RESET);
	HAL_Delay(1);
}




/**
* @brief  ��ȡ��ǰ�����������
* @param  power_mode�����������
* @return none
*/
void hero_chassis::Get_referee_power(Power_mode &power_mode){
	switch(Referee.GameRobotState.classis_power_limit){
		case 50:power_mode=power_50W;
				RF_power=50;
			break;
		case 55:power_mode=power_55W;
				RF_power=55;
			break;
		case 60:power_mode=power_60W;
				RF_power=60;
			break;
		case 65:power_mode=power_65W;
				RF_power=65;
			break;
		case 70:power_mode=power_70W;
				RF_power=70;	
			break;
		case 80:power_mode=power_80W;
				RF_power=80;
			break;
		case 90: power_mode=power_90W;
				RF_power=90;
			break;
		case 100:power_mode=power_100W;
				RF_power=100;
			break;
		case 120:power_mode=power_120W;
				RF_power=120;
			break;
		default: break;
	};
}



/**
* @brief  �������ƵĹ��������ٶȵ�λ
* @param  power_mode�����������
* @return none
*/
float test_speed=0.0;
void hero_chassis::speed_mode(Power_mode power_mode){
	switch(power_mode){
		case power_50W:
			Adjust_speed(2.5,4.0,3.2);
			max_speed=4000;
			break;
		case power_55W:
			Adjust_speed(2.675,4.5,3.525);
			max_speed=4500;
			break;
		case power_60W:
			Adjust_speed(2.85,5.0,3.85);
			max_speed=5000;
			break;
		case power_65W:
			Adjust_speed(3.025,5.25,4.175);
			max_speed=5250;
			break;
		case power_70W:
			Adjust_speed(5.0,6.3,5.5);
			max_speed=6300;
			break;
		case power_80W:
			Adjust_speed(4.0,6.4,5.7);
			max_speed=6400;
			break;
		case power_90W:
			Adjust_speed(5.0,6.95,6.25);
			max_speed=6600;
			break;
		case power_100W:
			Adjust_speed(5.5,7.0,6.8);
			max_speed=7000;
			break;
		case power_120W:
			Adjust_speed(6.0,8.0,7.5);
			max_speed=8000;
			break;
	}
}


/**
 * @brief  �������ã�����ʱ����һ������
 * @param  flag:��̨���͵ĳ��ٱ�־λ��power_mode�����ʵ�λ
 * @return 0��������   1������
 */
int hero_chassis::unlimit_set(bool flag,Power_mode &power_mode){
	if(!flag)	return 0;
	else {
		switch(power_mode){
			/*0��*/
			case power_50W:	power_mode=power_70W;
				motor_power=70;
				break;
			
			/*��������*/
			case power_70W: power_mode=power_90W;
				motor_power=90;
				break;
			case power_90W: power_mode=power_120W;
				motor_power=120;
				break;
			
			/*Ѫ������*/
			case power_55W:power_mode=power_70W;
				motor_power=70;
				break;
			case power_60W:power_mode=power_70W;
				motor_power=70;
				break;
			case power_65W:power_mode=power_80W;
				motor_power=80;
				break;
			default: break;
		}
	}
	return 1;
}


/**
* @brief  ���µ�ǰ����ʣ������ٶȵ�λ
* @param  power_mode�����������
* @return 
*/
void hero_chassis::update_power_data(){
	Get_referee_power(_power_mode);
	Set_target_power();
	unlimit_set(unlimit_power,_power_mode);
	speed_mode(_power_mode);
}



/**
 * @brief  ���ù������Ƶ�Ŀ�깦��,���ݵ�ǰ״̬�����Ƿ񳬹���
 * @param  none
 * @return none
 */
void hero_chassis::Set_target_power(){
	static bool temp;
	motor_power=RF_power;
	if(SourceManage.capObj.Voltage <= 16)	//�ͻرȽ�
	{
		temp = 0;
	}
	if(SourceManage.capObj.Voltage >= 19)
	{
		temp = 1;
	}
	if(!temp){
		unlimit_power=false;		//���ݵ�ѹ���ͣ������رճ���ģʽ
		unlimit_response=0;			//���ݵ�ѹ���ͣ��ظ���̨�����ü��ټ�
	}
	else{
		unlimit_response=1;
//		if((LF.Target+RF.Target+LB.Target+RB.Target)/(LF.Current+RF.Current+LB.Current+RB.Current)>2.0&&(SourceManage.capObj.Voltage>20)){
			if(SourceManage.capObj.Voltage>19){
			motor_power=motor_power+=20;
		}
	}
}


/**
* @brief  �������ƺ���
* @param  none
* @return none
*/
void hero_chassis::power_control(){

	PowerCtrl.Set_PE_Target(RF_power,motor_power,30);
	int temp_out[4];
	for(int i=0;i<4;i++){
		temp_out[i]=(int)My_ABS(wheel_Motor[i].Out);
	}
	PowerCtrl.Control(SourceManage.power.pow_In,SourceManage.power.pow_motor,
	Referee.PowerHeatData.chassis_power_buffer,temp_out);
	pow_scale=PowerCtrl.Get_limScale();		
	for(int i=0;i<4;i++){
		wheel_Motor[i].Out*=pow_scale;
	}
}



/**
 * @brief  UI�����־λ����
 * @param  total_flag��������̨���͵ı�־λ�ܺͣ�һ��uint8_t����8����־λ��
 * @return none
 */
void hero_chassis::UI_flag_update(uint8_t total_flag){
	UI_flag.B_flag=total_flag&1;
	UI_flag.Q_flag=(total_flag>>1)&1;
	UI_flag.C_flag=(total_flag>>2)&1;
	UI_flag.V_flag=(total_flag>>3)&1;
	UI_flag.shift_flag=(total_flag>>4)&1;
	
}