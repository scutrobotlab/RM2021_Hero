/**
  ***********************************************************************************
  * @file   : Service_Communication.cpp
  * @brief  : Communication support file.This file provides access ports to interface
  *           with connected devices.
  ***********************************************************************************
                                 ##### Port List #####
  =================================================================================
  |Port Name     Physical-Layer     Data-Link-Layer    Application-Layer    Number
  |————————————————————————————————————————
  |EXAMPLE_Port       CAN1               CAN                CUSTOM            0
  |CAN2_Port          CAN2               CAN                Custom            1
  |EBUG_Port         USART1             Custom              Custom            2
  |USART2_Port       USART2              DBUS               DJI-DR16          3
  *
**/
/* Includes ------------------------------------------------------------------*/
#include "Service_Communication.h"
#include "Chassis_control.h"
#include "Hero_Gimbal.h"
#include "Service_Devices.h"
#include "shoot.h"
/* Private define ------------------------------------------------------------*/
void Task_UsartRecieve(void *arg);
void Task_CAN1Transmit(void *arg);
void Task_CAN2Transmit(void *arg);
void Task_CAN1Receive(void *arg);
void Task_CAN2Receive(void *arg);



/*----------------------------------------------- CAN Manager ---------------------------------------------*/
/*Task Define ---------------------------*/
TaskHandle_t CAN1TxPort_Handle;
TaskHandle_t CAN2TxPort_Handle;
TaskHandle_t CAN1RxPort_Handle;
TaskHandle_t CAN2RxPort_Handle;

/*Function Prototypes--------------------*/
static void Convert_Data(CAN_RxMessage* input, CAN_COB* output);


/**
* @brief  Initialization of communication service
* @param  None.
* @return None.
*/
void Service_Communication_Init(void)
{
  /* CAN Filter Config*/
	CAN_Filter_Mask_Config(&hcan2, CanFilter_18 | CanFifo_1 | Can_STDID | Can_DataType, 0x207, 0x3FF);			//拨盘
	CAN_Filter_Mask_Config(&hcan1, CanFilter_2 	| CanFifo_1 | Can_STDID | Can_DataType, 0x201, 0x3FF);			//摩擦轮
	CAN_Filter_Mask_Config(&hcan1, CanFilter_3 	| CanFifo_1 | Can_STDID | Can_DataType, 0x202, 0x3FF);			//摩擦轮
	CAN_Filter_Mask_Config(&hcan1, CanFilter_4 	| CanFifo_1 | Can_STDID | Can_DataType, 0x203, 0x3FF);			//摩擦轮
	CAN_Filter_Mask_Config(&hcan1, CanFilter_6 	| CanFifo_1 | Can_STDID | Can_DataType, 0x205, 0x3FF);			//pitch电机
	CAN_Filter_Mask_Config(&hcan2, CanFilter_16 | CanFifo_1 | Can_STDID | Can_DataType, 0x206, 0x3FF);			//yaw电机
	CAN_Filter_Mask_Config(&hcan2, CanFilter_19 | CanFifo_1 | Can_STDID | Can_DataType, 0x112, 0x3FF);			//板间通信
	CAN_Filter_Mask_Config(&hcan2, CanFilter_20 | CanFifo_1 | Can_STDID | Can_DataType, 0x113, 0x3FF);			//板间通信
	xTaskCreate(Task_UsartRecieve,		"Com.Usart RxPort" , 	Normal_Stack_Size,    	NULL, PriorityAboveNormal,   	&UsartRxPort_Handle);
	xTaskCreate(Task_CAN1Receive,		"Com.CAN1 RxPort" , 	Small_Stack_Size,    	NULL, PriorityAboveNormal,   	&CAN1RxPort_Handle);
	xTaskCreate(Task_CAN2Receive,		"Com.CAN2 RxPort" , 	Small_Stack_Size,    	NULL, PriorityAboveNormal,   	&CAN2RxPort_Handle);
	xTaskCreate(Task_CAN1Transmit,		"Com.CAN TxPort" , 		Small_Stack_Size,    	NULL, PriorityHigh,   			&CAN1TxPort_Handle);
	xTaskCreate(Task_CAN2Transmit,		"Com.CAN TxPort" , 		Small_Stack_Size,    	NULL, PriorityHigh,   			&CAN2TxPort_Handle);

}
/**
* @brief  Tasks for CAN Management.管理CAN的发送，用户只需要往邮箱里填入message即可
* @param  None.
* @return None.
*/
void Task_CAN1Transmit(void *arg)
{
  /* Cache for Task */
  uint8_t free_can_mailbox;
  CAN_COB CAN_TxMsg;
  /* Pre-Load for task */
  
  /* Infinite loop */
  
  for(;;)
  {
    /* CAN1 Send Port */
    if(xQueueReceive(CAN1_TxPort,&CAN_TxMsg,portMAX_DELAY) == pdPASS)
    {
      do{
        free_can_mailbox = HAL_CAN_GetTxMailboxesFreeLevel(&hcan1);
      }while(free_can_mailbox == 0);
      CANx_SendData(&hcan1,CAN_TxMsg.ID,CAN_TxMsg.Data,CAN_TxMsg.DLC);
    }
  }
}



void Task_CAN2Transmit(void *arg)
{
  /* Cache for Task */
  uint8_t free_can_mailbox;
  CAN_COB CAN_TxMsg;
  /* Pre-Load for task */
  
  /* Infinite loop */
  
  for(;;)
  {
    /* CAN2 Send Port */
    if(xQueueReceive(CAN2_TxPort,&CAN_TxMsg,portMAX_DELAY) == pdPASS)
    {
      do{
        free_can_mailbox = HAL_CAN_GetTxMailboxesFreeLevel(&hcan2);
      }while(free_can_mailbox == 0);
      CANx_SendData(&hcan2,CAN_TxMsg.ID,CAN_TxMsg.Data,CAN_TxMsg.DLC);
    }
  }
}



void Task_CAN1Receive(void *arg)
{
	/* Cache for Task */
	CAN_COB CAN_RxMsg;

	/* Pre-Load for task */

	/* Infinite loop */

	for(;;)
	{
		/* CAN1 Receive Port */
		if(xQueueReceive(CAN1_RxPort,&CAN_RxMsg,portMAX_DELAY) == pdPASS)
		{
			/* 更新CAN1云台电机数据 */
			Hero_Gimbal.CAN1_MotorUpdate(&CAN_RxMsg);
		}
	}
}

void Task_CAN2Receive(void *arg)
{
	
	/* Cache for Task */
	CAN_COB CAN_RxMsg;

	/* Pre-Load for task */

	/* Infinite loop */

	for(;;)
	{
		/* CAN2 Receive Port */
		if(xQueueReceive(CAN2_RxPort, &CAN_RxMsg, portMAX_DELAY) == pdPASS)
		{
			/* 更新CAN2云台电机和底盘传输过来的数据 */
			Hero_Gimbal.CAN2_MotorUpdate(&CAN_RxMsg);
		}
	}
}


   /* 接收中断函数 ------------------------------------------*/
/**
	* @brief  CAN1接收中断，控制摩擦轮和pitch电机
  * @param 
	* @retval 
	* @retval 
	*/
void User_CAN1_RxCpltCallback(CAN_RxBuffer *CAN_RxMessage)
{
	static CAN_COB   CAN_RxCOB;
	Convert_Data(CAN_RxMessage,&CAN_RxCOB);
	
	
	//Send To CAN Receive Queue
	if(CAN1_RxPort != NULL)
	xQueueSendFromISR(CAN1_RxPort, &CAN_RxCOB, 0);
}

/**
	* @brief  CAN2接收中断，控制拨盘和yaw电机
  * @param 
	* @retval 
	* @retval 
	*/
void User_CAN2_RxCpltCallback(CAN_RxBuffer *CAN_RxMessage)
{
	static CAN_COB   CAN_RxCOB;
	Convert_Data(CAN_RxMessage,&CAN_RxCOB);

	//Send To CAN Receive Queue
	if(CAN2_RxPort != NULL)
	xQueueSendFromISR(CAN2_RxPort, &CAN_RxCOB, 0);
}

/**
* @brief  Data convertion，provide lower layer access port 
          for application layer.
* @param  CAN_RxMsg：Lower layer CAN frame.
          CAN_RxCOB：Application layer CAN Frame.
* @return None.
*/
static void Convert_Data(CAN_RxMessage* input, CAN_COB* output)
{
  output->ID = input->header.StdId;
  output->DLC = input->header.DLC;
  memcpy(output->Data, input->data, output->DLC);
}


/*---------------------------------------------- USART Manager --------------------------------------------*/


/*Function Prototypes--------------------*/
TaskHandle_t UsartRxPort_Handle;


uint32_t DR16_call_back(uint8_t* Recv_Data, uint16_t ReceiveLen)
{
	static BaseType_t* pxHigherPriorityTaskWoken;
	
	if(DeviceDR16_Handle != NULL && ReceiveLen == 18)
	{
		xTaskNotifyFromISR(DeviceDR16_Handle, (uint32_t) Recv_Data, eSetValueWithOverwrite, pxHigherPriorityTaskWoken);
	}
  return 0;
}



uint32_t User_UART4_RxCpltCallback(uint8_t* Recv_Data, uint16_t ReceiveLen)
{
  return 0;
}



uint32_t GetViaionData(uint8_t* Recv_Data, uint16_t ReceiveLen)		//ReceiveLen实际数据长度
{
	static USART_COB Usart_RxCOB;
	//Send To UART Receive Queue
	if(USART_RxPort != NULL){
		Usart_RxCOB.port_num = 6;
		Usart_RxCOB.len      = ReceiveLen;
		Usart_RxCOB.address  = Recv_Data;
		xQueueSendFromISR(USART_RxPort,&Usart_RxCOB,0);
	}
	return 0;
}


/**
* @brief  Tasks for USART Management.
          Attention:In this version we passing the pointer of data not copying
          data itself and we only have one buff zone, so user need to process 
          the data as soon as possible in case of over-write.
* @param  None.
* @return None.
*/
void Task_UsartRecieve(void *arg)
{
	USART_COB Usart_RxCOB;
	for(;;){
    /* CAN1 Send Port */ 
		if(xQueueReceive(USART_RxPort,&Usart_RxCOB,portMAX_DELAY) == pdPASS){
			switch(Usart_RxCOB.port_num){
				case 2:
//					DR16.DataCapture((DR16_DataPack_Typedef*)Usart_RxCOB.address);
					break;
				case 3:
					break;
				case 4:
					break;
				case 5:
					break;
				case 6:
					memcpy(Hero_Gimbal.PCvision.PackFromVisionUnion.UsartData,Usart_RxCOB.address,12);
					if((Hero_Gimbal.Chassis.UI_flag.mouse_R_flag==1)&&(Hero_Gimbal.PCvision.PC_CheckLinkFlag)&&(Hero_Gimbal.PCvision.PackFromVisionUnion.PackFromVision.flag==72)){
						Hero_Gimbal.Pitchyaw.Vision_Pit_Target(0.8*Hero_Gimbal.PCvision.PackFromVisionUnion.PackFromVision.pitchData);
						Hero_Gimbal.Pitchyaw.Vision_Yaw_Target(-1.4*Hero_Gimbal.PCvision.PackFromVisionUnion.PackFromVision.yawData);
					}
					break;
				default:
					
					break;
			}
		}
	}
}

/************************ COPYRIGHT(C) SCUT-ROBOTLAB **************************/
