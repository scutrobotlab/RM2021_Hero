/**
  ******************************************************************************
  * @file   System_config.cpp
  * @brief  Deploy resources,tasks and services in this file.
  ******************************************************************************
  * @note
  *  - Before running your Task you should first include your headers and init- 
  *    ialize used resources in "System_Resource_Init()". This function will be 
  *    called before tasks Start.
  *    
  *  - All tasks should be created in "System_Tasks_Init()", this function will
  *    be called in 'defaultTask()'.
  *
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
#include "System_Config.h"
#include "System_DataPool.h"
#include "Hero_config.h"

/* Service */
#include "Service_Devices.h"
#include "Service_Debug.h"
#include "Service_SwerveChassis.h"
#include "Service_Communication.h"
/* User support package & SRML */
#include <SRML.h>
#include "SEGGER_SYSVIEW.h"
/* Private variables ---------------------------------------------------------*/


/*Founctions------------------------------------------------------------------*/
/**
* @brief Load drivers ,modules, and data resources for tasks.
* @note  Edit this function to add Init-functions and configurations.
*/
void System_Resource_Init(void)
{
  /* Drivers Init ---------------------*/
  Timer_Init(&htim4, USE_HAL_DELAY);
  
  CAN_Init(&hcan1,&User_CAN1_RxCpltCallback);
  CAN_Init(&hcan2,&User_CAN2_RxCpltCallback);
  CAN_Filter_Mask_Config(&hcan2, CanFilter_19 | CanFifo_1 | Can_STDID | Can_DataType, 0x201, 0x3FF);			//LF
  CAN_Filter_Mask_Config(&hcan2, CanFilter_20 | CanFifo_1 | Can_STDID | Can_DataType, 0x202, 0x3FF);			//RF
  CAN_Filter_Mask_Config(&hcan2, CanFilter_21 | CanFifo_1 | Can_STDID | Can_DataType, 0x203, 0x3FF);			//RB
  CAN_Filter_Mask_Config(&hcan2, CanFilter_22 | CanFifo_1 | Can_STDID | Can_DataType, 0x204, 0x3FF);			//LB
  CAN_Filter_Mask_Config(&hcan1, CanFilter_2 | CanFifo_1 | Can_STDID | Can_DataType, 0x111, 0x3FF);			//∞Âº‰Õ®–≈
  
  Uart_Init(&huart1, Uart1_Rx_Buff, USART1_RX_BUFFER_SIZE,RecHandle);
  Uart_Init(&huart6, Uart6_Rx_Buff, USART6_RX_BUFFER_SIZE,Referee_recv_Callback);

  

  /* RTOS resources Init --------------*/
  USART_RxPort       = xQueueCreate(4,sizeof(USART_COB));
  USART_TxPort       = xQueueCreate(2,sizeof(USART_COB));
  CAN1_TxPort        = xQueueCreate(4,sizeof(COB_TypeDef));
  CAN2_TxPort        = xQueueCreate(4,sizeof(COB_TypeDef));
  RMMotor_QueueHandle= xQueueCreate(10,sizeof(COB_TypeDef));
  IMU_QueueHandle    = xQueueCreate(2,sizeof(COB_TypeDef));
  NUC_QueueHandle    = xQueueCreate(2,sizeof(USART_COB));
  DR16_QueueHandle   = xQueueCreate(2,sizeof(USART_COB));
  Referee_QueueHandle = xQueueCreate(2,sizeof(USART_COB));
  
  /* Other resources Init -------------*/
  
  
  /* Modules Init ---------------------*/
  SEGGER_SYSVIEW_Conf();
  myPIDTimer::getMicroTick_regist(Get_SystemTimer);

  
  /* Service configurations -----------*/
  
}  


/**
* @brief Load and start User Tasks. 
* @note  Edit this function to add tasks into the activated tasks list.
*/
void System_Tasks_Init(void)
{ 
  /* Syetem Service init --------------*/

  Service_Debug_Init();
  Service_Devices_Init();
  Service_Communication_Init();
	  
	Hero_Config_Init();
  /* Applications Init ----------------*/
  xTaskCreate(Task_SwerveChassis, "Dev.Actuator" , Tiny_Stack_Size,    NULL, PrioritySuperHigh,   &TestSwerveChassis_Handle);
}


/************************ COPYRIGHT(C) SCUT-ROBOTLAB **************************/

