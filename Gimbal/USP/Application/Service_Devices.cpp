/**
  ******************************************************************************
  * @file   Service_Devices.cpp
  * @brief  Devices service running file.
  ******************************************************************************
  * @note
  *  - Before running your devices, just do what you want ~ !
  *  - More devices or using other classification is decided by yourself ~ !
  ===============================================================================
                                    Task List
  ===============================================================================
  * <table>
  * <tr><th>Task Name     <th>Priority          <th>Frequency/Hz    <th>Stack/Byte
  * <tr><td>              <td>                  <td>                <td>    
  * </table>
  *
 */
/* Includes ------------------------------------------------------------------*/
#include "Service_Devices.h"
#include "Hero_Gimbal.h"
#include "Remote_Keyboard.h"
/* Private define ------------------------------------------------------------*/
TaskHandle_t DeviceDR16_Handle;
TaskHandle_t DeviceControl_Handle;
TaskHandle_t KeyboardCtrl_Handle;
TaskHandle_t RemoteCtrl_Handle;
TaskHandle_t ModeSet_Handle;
TaskHandle_t Pc_Handle;
TaskHandle_t IMU_Handle;
TaskHandle_t Indicator_Handle;
/* Private variables ---------------------------------------------------------*/
/* Private function declarations ---------------------------------------------*/
void Device_DR16(void *arg);
void Device_Control(void *arg);
void Mode_Set(void *arg);
void Pc_Sent(void *arg);
void IMU_Task(void *arg);
void Indicator_Task(void *arg);
/* Exported devices ----------------------------------------------------------*/
/* Motor & ESC & Other actuators*/
//Motor_AK80_9  Test_Motor(1, 0, 0);
/* Remote control */

/* IMU & NUC & Other sensors */

/* Other boards */

/* Function prototypes -------------------------------------------------------*/
/**
* @brief  Initialization of device management service
* @param  None.
* @return None.
*/
void Service_Devices_Init(void)
{
	xTaskCreate(Device_DR16,      	"Dev.DR16", 		Small_Stack_Size,  	NULL, 	PriorityHigh,   		&DeviceDR16_Handle);
	xTaskCreate(Device_Control,   	"Dev.Control", 		Small_Stack_Size,   NULL, 	PriorityHigh,   		&DeviceControl_Handle);
	xTaskCreate(Mode_Set,			"Mode_set",			Small_Stack_Size,	NULL,	PriorityHigh,			&ModeSet_Handle);
	xTaskCreate(Pc_Sent,			"Pc_Sent",			Small_Stack_Size,	NULL,	PriorityHigh,			&Pc_Handle);
	xTaskCreate(IMU_Task,			"imu",				Large_Stack_Size,	NULL,	PriorityHigh,			&IMU_Handle);
	xTaskCreate(Indicator_Task,		"indicator",		Small_Stack_Size,	NULL,	PriorityHigh,			&Indicator_Handle);
}








void Device_DR16(void *arg)
{
  /* Cache for Task */

  /* Pre-Load for task */

  /* Infinite loop */
	DR16_DataPack_Typedef* dr16_pack;
	TickType_t _xTicksToWait = pdMS_TO_TICKS(1);
	for(;;)
	{
		
		if(xTaskNotifyWait(0x00000000, 0xFFFFFFFF, (uint32_t *) &dr16_pack, 0) == pdTRUE)
			{
				/* Remote control data unpacking */
				DR16.DataCapture(dr16_pack);
			}
	}
}


/**
 * @brief  判断操作模式，进而通知键鼠控制或者遥控器控制
 * @param  none
 * @return none
 */
void Mode_Set(void *arg){
	TickType_t xLastWakeTime_t;
    xLastWakeTime_t = xTaskGetTickCount();
	LinkageStatus_Typedef last_status;
	for(;;){
		if(DR16.GetStatus()==DR16_ESTABLISHED){
			if(last_status==DR16_LOST)
				Hero_Gimbal.Pitchyaw.reset_motor();		//遥控器打开，云台归中
			else if(last_status==DR16_ESTABLISHED){
				if((DR16.GetS1()==DR16_SW_DOWN)&&(DR16.GetS2()==DR16_SW_DOWN)){
					Keyboard_Chassial_Ctrl();
					Keyboard_SuperCap_Ctrl();
					Keyboard_Shoot_Ctrl();
					Keyboard_PitchYaw_Ctrl();
					Keyboard_flag_Ctrl();
					Keyboard_Upslope();
					Keyboard_ChangeTarget();
					Keyboard_Reset();
				}
				else {
					Remote_Chassial_Ctrl();
					Remote_Gimbal_Ctrl();
					Remote_Shoot_Ctrl();
				}
			}
		}
		last_status=DR16.GetStatus();
		vTaskDelayUntil(&xLastWakeTime_t, 1);
	}
	
}



/**
 * @brief  云台控制任务
 * @param  none
 * @return none
 */
void Device_Control(void *arg){
	TickType_t xLastWakeTime_t;
    xLastWakeTime_t = xTaskGetTickCount();
	for(;;){
		DR16.Check_Link(xTaskGetTickCount());
		Hero_Gimbal.update();

		if(DR16.GetStatus()==DR16_ESTABLISHED){
			Hero_Gimbal.control();
		}
		else 
			Hero_Gimbal.stop();

		vTaskDelayUntil(&xLastWakeTime_t, 1);
	}
}



void Pc_Sent(void *arg)
{
	TickType_t _xPreviousWakeTime = xTaskGetTickCount();
	TickType_t _xTimeIncrement = pdMS_TO_TICKS(1);
	uint8_t PC_count=0;	//计数器，每10ms检测一次小电脑数据是否异常
	uint16_t last_bullet = 10;
	
	Hero_Gimbal.Chassis.Referee.shooter_id1_42mm_speed_limit = last_bullet;
	
	for (;;){
		
		if(!Hero_Gimbal.Chassis.Referee.shooter_id1_42mm_speed_limit)
		{
			Hero_Gimbal.Chassis.Referee.shooter_id1_42mm_speed_limit = last_bullet;
		}
		else
			last_bullet = Hero_Gimbal.Chassis.Referee.shooter_id1_42mm_speed_limit;
		
		Hero_Gimbal.PCvision.SendGimbleStatus(-Hero_Gimbal.Pitchyaw.Pitch_ang.Current,
		Hero_Gimbal.Pitchyaw.Yaw_ang.Current,Hero_Gimbal.Chassis.Referee.robot_id,
		Hero_Gimbal.Chassis.Referee.shooter_id1_42mm_speed_limit,ChangeTarget,0);
		if(PC_count<=20) 
			PC_count++;
		
		else{
			PC_count=0;
			Hero_Gimbal.PCvision.PC_CheckLink();
		}
		vTaskDelayUntil(&_xPreviousWakeTime, _xTimeIncrement);
	}
}

/**
 * @brief  IMU任务

 */
void IMU_Task(void *arg)
{
	TickType_t _xPreviousWakeTime = xTaskGetTickCount();
	TickType_t _xTimeIncrement = pdMS_TO_TICKS(1);
	
	for (;;){
		MPU_Get_Accelerometer(&MPU6050_IIC_PIN, &MPUData.ax, &MPUData.ay, &MPUData.az);
		if(!MPU_Get_Gyroscope(&MPU6050_IIC_PIN, &MPUData.gx, &MPUData.gy, &MPUData.gz))
		{
			MPUData.gx -= MPUData.gxoffset;
			MPUData.gy -= MPUData.gyoffset;
			MPUData.gz -= MPUData.gzoffset;
		}
		mpu_dmp_get_data(&MPUData.pitch, &MPUData.roll, &MPUData.yaw);	
		vTaskDelayUntil(&_xPreviousWakeTime, _xTimeIncrement);
	}	
}


		/*指示灯任务*/
void Indicator_Task(void *arg){

	TickType_t _xPreviousWakeTime = xTaskGetTickCount();
	TickType_t _xTimeIncrement = pdMS_TO_TICKS(10);
	Hero_Gimbal.indicator.Reset();
	uint16_t LED6_cnt=0;
	for (;;){	
		
			/*LED1，电容容量*/
		Hero_Gimbal.indicator.Cap_Energy_Indicator(CAP_Voltage, 24, 22, 16, LED_1, 0x80);
			
		
			/*LED2，发射状态*/
		if(indicator_State.shoot_state==0){
			Hero_Gimbal.indicator.Change_Singal_RGB(LED_2,0,0xff,0,0x80);	//绿灯
		}
		else if(indicator_State.shoot_state==1){
			Hero_Gimbal.indicator.Change_Singal_RGB(LED_2,0xff,0,0,0x80);	//红灯
		}
		else if(indicator_State.shoot_state==2){
			Hero_Gimbal.indicator.Change_Singal_RGB(LED_2,0,0,0,0x80);	//灭灯
		}
		else if(indicator_State.shoot_state==3){
			Hero_Gimbal.indicator.Change_Singal_RGB(LED_2,0,0,0xff,0x80);//蓝灯
		}
		
		
			/*LED3，小陀螺*/
		if(indicator_State.rotate_state==0){
			Hero_Gimbal.indicator.Change_Singal_RGB(LED_3,0,0,0,0x80);	//灭灯
		}
		else Hero_Gimbal.indicator.Change_Singal_RGB(LED_3,0,0xff,0,0x80);//绿灯
		
		
			/*LED4，一键掉头，转90°*/
		if(indicator_State.shoot90_state==0){
			Hero_Gimbal.indicator.Change_Singal_RGB(LED_4,0,0,0,0x80);//灭灯
		}
		else if(indicator_State.shoot90_state==1){
			Hero_Gimbal.indicator.Change_Singal_RGB(LED_4,0,0,0xff,0x80);//蓝灯
		}	
		else  if(indicator_State.shoot90_state==2){
			Hero_Gimbal.indicator.Change_Singal_RGB(LED_4,0xff,0xff,0,0x80);	//黄灯
		}
		
		
			/*LED5，加速，上坡指示*/
		if((!DR16.IsKeyPress(DR16_KEY_V))&&(!DR16.IsKeyPress(DR16_KEY_C))){
			indicator_State.Super_CAP=0;
		}
		if(indicator_State.Super_CAP==0){
			Hero_Gimbal.indicator.Change_Singal_RGB(LED_5,0,0,0,0x80);	//灭灯
		}
		else if(indicator_State.Super_CAP==1){
			Hero_Gimbal.indicator.Change_Singal_RGB(LED_5,0xff,0xf0,0xf0,0x80);	//白灯
		}
		else Hero_Gimbal.indicator.Change_Singal_RGB(LED_5,0x84,0x70,0xff,0x80);	//紫灯
		
		
			/*LED6，自瞄状态指示*/
					/*自瞄在线模式*/
		if(Hero_Gimbal.PCvision.PC_CheckLinkFlag){
			Hero_Gimbal.indicator.Change_Singal_RGB(LED_6,0,0xff,0,0x80);//绿灯
		}
					/*自瞄不在线*/
		else if(!Hero_Gimbal.PCvision.PC_CheckLinkFlag){
			Hero_Gimbal.indicator.Change_Singal_RGB(LED_6,0xff,0,0,0x80);	//红灯
		}
		
		Hero_Gimbal.indicator.Update();
	
		vTaskDelayUntil(&_xPreviousWakeTime, _xTimeIncrement);
	}	

}

/* User Code End Here ---------------------------------*/

/************************ COPYRIGHT(C) SCUT-ROBOTLAB **************************/
