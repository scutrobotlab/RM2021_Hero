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

/* Private define ------------------------------------------------------------*/

TaskHandle_t DeviceSourceManage_Handle;
TaskHandle_t DeviceSensors_Handle;
TaskHandle_t RecvReferee_Handle;
/* Private variables ---------------------------------------------------------*/
/* Private function declarations ---------------------------------------------*/

void Device_Sensors(void *arg);
void Device_SourceManage(void *arg);
void Recv_Referee(void *arg);
/* Exported devices ----------------------------------------------------------*/
/* Motor & ESC & Other actuators*/

/* Remote control */

/* IMU & NUC & Other sensors */

/* Other boards */

/* Function prototypes -------------------------------------------------------*/
/*
* @brief  Initialization of device management service
* @param  None.
* @return None.
*/
void Service_Devices_Init(void)
{
	xTaskCreate(Device_SourceManage,   "Dev.SourceManage"  , Small_Stack_Size,    NULL, PriorityRealtime,        &DeviceSourceManage_Handle);
	xTaskCreate(Device_Sensors,   "Dev.Sensors"  , Tiny_Stack_Size,    NULL, PriorityRealtime,        &DeviceSensors_Handle);
	xTaskCreate(Recv_Referee,    "Rx.Referee"   , Small_Stack_Size,    NULL, PriorityRealtime,  &RecvReferee_Handle);
}





void Device_Sensors(void *arg)
{
  /* Cache for Task */
  
  /* Pre-Load for task */

  /* Infinite loop */
  TickType_t xLastWakeTime_t;
  xLastWakeTime_t = xTaskGetTickCount();
  for(;;)
  {
    /* Read IMU Message */
    
    /* Exchange NUC Meaasge */
    
    /* Read Other board Message */

    
  /* Pass control to the next task ------------------------------------------*/
    vTaskDelayUntil(&xLastWakeTime_t,2);
  }
}

void Device_SourceManage(void *arg)
	{
		TickType_t xLastWakeTime_t = xTaskGetTickCount();

		HAL_ADC_Start_DMA(&hadc1, (uint32_t *)ADCReadBuff, 7);
		HAL_DAC_Start(&hdac, DAC_CHANNEL_1);
		Hero_chassis.SourceManage.SourceManage_Init();
		for(;;)
		{
			//更新数据
			Hero_chassis.SourceManage.Update(ADCReadBuff);
			Hero_chassis.SourceManage.Set_ChargePower(PowerCtrl.Get_capChargePower());
			Hero_chassis.SourceManage.Manage();	
			HAL_DAC_SetValue(&hdac, DAC1_CHANNEL_1, DAC_ALIGN_12B_R, Hero_chassis.SourceManage.capObj.charge_DAC_Value);
			vTaskDelayUntil(&xLastWakeTime_t,10);
		}
	}
			
			
			
void Recv_Referee(void *arg){
	/* Pre-Load for task */
	static USART_COB* referee_pack;
	static TickType_t xLastWakeTime_t = xTaskGetTickCount();
	/* Infinite loop */
	for(;;){
		if(xTaskNotifyWait(0x00000000, 0xFFFFFFFF, (uint32_t *) &referee_pack, 0) == pdTRUE){
		Referee.unPackDataFromRF((uint8_t*)referee_pack->address, referee_pack->len);
	}
	/* Pass control to the next task */
	vTaskDelayUntil(&xLastWakeTime_t,1);
	}
}
/* User Code End Here ---------------------------------*/

/************************ COPYRIGHT(C) SCUT-ROBOTLAB **************************/
