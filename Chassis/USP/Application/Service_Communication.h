/**
  **********************************************************************************
  * @file   ：Service_Communication.h
  * @brief  ：Header of Service_Communication.cpp
  **********************************************************************************
**/
#ifndef  _SERVICE_COMMUNICATE_H_
#define  _SERVICE_COMMUNICATE_H_

#include "System_DataPool.h"
#include "Hero_chassis.h"
#include "move_task.h"
#include "Service_Devices.h"

#ifdef  __cplusplus
extern "C"{
#endif
/*------------------------------ System Handlers ------------------------------*/
extern TaskHandle_t CAN1SendPort_Handle;
extern TaskHandle_t CAN2SendPort_Handle;
extern TaskHandle_t CANReceivePort_Handle;
extern TaskHandle_t UartTransmitPort_Handle;

/*------------------------------Function prototypes ---------------------------*/
uint32_t User_UART2_RxCpltCallback(uint8_t* Recv_Data, uint32_t ReceiveLen);
uint32_t User_UART3_RxCpltCallback(uint8_t* Recv_Data, uint32_t ReceiveLen);
uint32_t User_UART4_RxCpltCallback(uint8_t* Recv_Data, uint32_t ReceiveLen);
uint32_t User_UART5_RxCpltCallback(uint8_t* Recv_Data, uint32_t ReceiveLen);
void User_CAN1_RxCpltCallback(CAN_RxBuffer *CAN_RxMessage);
void User_CAN2_RxCpltCallback(CAN_RxBuffer *CAN_RxMessage);
void Service_Communication_Init(void);
uint32_t Referee_recv_Callback(uint8_t* Recv_Data, uint16_t ReceiveLen);
	
#ifdef  __cplusplus
}
#endif
#endif  

/************************* COPYRIGHT SCUT-ROBOTLAB *****END OF FILE****************/

