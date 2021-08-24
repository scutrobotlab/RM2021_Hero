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
/* Private define ------------------------------------------------------------*/
void Task_CAN1Transmit(void *arg);
void Task_CAN2Transmit(void *arg);

/**
* @brief  Initialization of communication service
* @param  None.
* @return None.
*/
void Service_Communication_Init(void)
{
  
  /* CAN Filter Config*/
//  CAN_Filter_Mask_Config(&hcan1, CanFilter_1|CanFifo_0|Can_STDID|Can_DataType,0x001,0x3ff);//筛选器:|编号|FIFOx|ID类型|帧类型|ID|屏蔽位(0x3ff,0x1FFFFFFF)|
//  CAN_Filter_Mask_Config(&hcan2, CanFilter_15|CanFifo_0|Can_STDID|Can_DataType,0x002,0x3ff);//筛选器:|编号|FIFOx|ID类型|帧类型|ID|屏蔽位(0x3ff,0x1FFFFFFF)|
//  CAN_Filter_Mask_Config(&hcan2, CanFilter_14|CanFifo_0|Can_STDID|Can_DataType,0x003,0x3ff);//筛选器:|编号|FIFOx|ID类型|帧类型|ID|屏蔽位(0x3ff,0x1FFFFFFF)|
//  

//  xTaskCreate(Task_CAN1Transmit, "Com.CAN1 TxPort"  , Tiny_Stack_Size,    NULL, PrioritySuperHigh,   &CAN1SendPort_Handle);
//  xTaskCreate(Task_CAN2Transmit, "Com.CAN2 TxPort"  , Tiny_Stack_Size,    NULL, PrioritySuperHigh,   &CAN2SendPort_Handle); 
}

/*----------------------------------------------- CAN Manager ---------------------------------------------*/
/*Task Define ---------------------------*/
TaskHandle_t CAN1SendPort_Handle;
TaskHandle_t CAN2SendPort_Handle;
TaskHandle_t CANReceivePort_Handle;
static void Convert_Data(CAN_RxMessage* input, COB_TypeDef* output);

/*Function Prototypes--------------------*/
///**
//* @brief  Tasks for CAN Management.
//* @param  None.
//* @return None.
//*/
//void Task_CAN1Transmit(void *arg)
//{
//  /* Cache for Task */
//  uint8_t free_can_mailbox;
//  COB_TypeDef CAN_TxMsg;
//  /* Pre-Load for task */
//  
//  /* Infinite loop */
//  
//  for(;;)
//  {
//    /* CAN1 Send Port */
//    if(xQueueReceive(CAN1_TxPort,&CAN_TxMsg,portMAX_DELAY) == pdPASS)
//    {
//      do{
//        free_can_mailbox = HAL_CAN_GetTxMailboxesFreeLevel(&hcan1);
//      }while(free_can_mailbox == 0);
//      CANx_SendData(&hcan1,CAN_TxMsg.ID,CAN_TxMsg.Data,CAN_TxMsg.DLC);
//    }
//  }
//}

//void Task_CAN2Transmit(void *arg)
//{
//  /* Cache for Task */
//  uint8_t free_can_mailbox;
//  COB_TypeDef CAN_TxMsg;
//  /* Pre-Load for task */
//  
//  /* Infinite loop */
//  
//  for(;;)
//  {
//    /* CAN2 Send Port */
//    if(xQueueReceive(CAN2_TxPort,&CAN_TxMsg,portMAX_DELAY) == pdPASS)
//    {
//      do{
//        free_can_mailbox = HAL_CAN_GetTxMailboxesFreeLevel(&hcan2);
//      }while(free_can_mailbox == 0);
//      CANx_SendData(&hcan2,CAN_TxMsg.ID,CAN_TxMsg.Data,CAN_TxMsg.DLC);
//    }
//  }
//}


/**
* @brief  Callback function in CAN Interrupt
* @param  None.
* @return None.
*/
void User_CAN1_RxCpltCallback(CAN_RxBuffer *CAN_RxMessage)
{
	if(CAN_RxMessage->header.StdId == 0x111)  {	
		for(int i=0;i<8;i++){
			gimbal_ctrl[i]=CAN_RxMessage->data[i];//板间通信
		}			
	}
}

void User_CAN2_RxCpltCallback(CAN_RxBuffer *CAN_RxMessage)
{

	 if(CAN_RxMessage->header.StdId == 0x201)  			
	{
		wheel_Motor[0].update(CAN_RxMessage->data);  //左前轮电机
	}
	else if(CAN_RxMessage->header.StdId == 0x202)  
	{
		wheel_Motor[1].update(CAN_RxMessage->data);	//右前轮电机
	}
	else if(CAN_RxMessage->header.StdId == 0x203)  
	{
		wheel_Motor[2].update(CAN_RxMessage->data);	//右后轮电机
	}	
	else if(CAN_RxMessage->header.StdId == 0x204)  
	{
		wheel_Motor[3].update(CAN_RxMessage->data);	//左后轮电机
	}

}

/**
* @brief  Data convertion，provide lower layer access port 
          for application layer.
* @param  CAN_RxMsg：Lower layer CAN frame.
          CAN_RxCOB：Application layer CAN Frame.
* @return None.
*/
static void Convert_Data(CAN_RxMessage* input, COB_TypeDef* output)
{
  output->ID = input->header.StdId;
  output->DLC = input->header.DLC;
  memcpy(output->Data, input->data, output->DLC);
}


/*---------------------------------------------- USART Manager --------------------------------------------*/
/*Task Define ---------------------------*/
TaskHandle_t UartTransmitPort_Handle;

/*Function Prototypes--------------------*/
/**
* @brief  Tasks for USART Management.
          Attention:In this version we passing the pointer of data not copying
          data itself and we only have one buff zone, so user need to process 
          the data as soon as possible in case of over-write.
* @param  None.
* @return None.
*/
//void Task_UsartTransmit(void *arg)
//{
//  /* Cache for Task */
//  UART_HandleTypeDef* pUart_x;
//  static std::vector<uint8_t> Packed_COB;
//  static USART_COB  Usart_TxCOB;
//  /* Pre-Load for task */
//  /* Infinite loop */
//  for(;;)
//  {
//    /* Usart Receive Port*/
//    if(xQueueReceive(USART_TxPort,&Usart_TxCOB,portMAX_DELAY) == pdPASS)
//    {
//      /* User Code Begin Here -------------------------------*/
//      switch(Usart_TxCOB.port_num)
//      {
//        case 4:
//          pUart_x = &huart3;
//          break;
//        case 5:
//          pUart_x = &huart4;
//          break;
//      }
//      /* User Code End Here ---------------------------------*/
//      HAL_UART_Transmit_DMA(pUart_x,(uint8_t*)Usart_TxCOB.address,Usart_TxCOB.len);
//    }
//  }
//}

/**
* @brief  Callback function in USART Interrupt
* @param  None.
* @return None.
*/
/*
  DR16
*/
uint32_t User_UART2_RxCpltCallback(uint8_t* Recv_Data, uint32_t ReceiveLen)
{
  static USART_COB Usart_RxCOB;
  //Send To UART Receive Queue
  if(DR16_QueueHandle != NULL)
  {
    Usart_RxCOB.port_num = 3;
    Usart_RxCOB.len      = ReceiveLen;
    Usart_RxCOB.address  = Recv_Data;
    xQueueSendFromISR(DR16_QueueHandle,&Usart_RxCOB,0);
  }
  return 0;
}

uint32_t User_UART3_RxCpltCallback(uint8_t* Recv_Data, uint32_t ReceiveLen)
{
  return 0;
}

uint32_t User_UART4_RxCpltCallback(uint8_t* Recv_Data, uint32_t ReceiveLen)
{
  return 0;
}

uint32_t User_UART5_RxCpltCallback(uint8_t* Recv_Data, uint32_t ReceiveLen)
{
  return 0;
}


	/*这个是直接复制去年的，到时候真正写底盘功率的时候重写一遍*/
uint32_t Referee_recv_Callback(uint8_t* Recv_Data, uint16_t ReceiveLen){
	static BaseType_t* pxHigherPriorityTaskWoken;
	static USART_COB referee_pack;
//	static uint8_t temp_buff[256];
	
	if(RecvReferee_Handle != NULL && ReceiveLen < 256){
//		memcpy(temp_buff, Recv_Data, ReceiveLen);
//		referee_pack.address = temp_buff;
//		referee_pack.len = ReceiveLen;
		referee_pack.address = Recv_Data;
		referee_pack.len = ReceiveLen;
		xTaskNotifyFromISR(RecvReferee_Handle, (uint32_t) &referee_pack, eSetValueWithOverwrite, pxHigherPriorityTaskWoken);
	}
  return 0;
}
	
/************************ COPYRIGHT(C) SCUT-ROBOTLAB **************************/
