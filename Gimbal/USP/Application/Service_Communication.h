/**
  **********************************************************************************
  * @file   ：Service_Communication.h
  * @brief  ：Header of Service_Communication.cpp
  **********************************************************************************
**/
#ifndef  _SERVICE_COMMUNICATE_H_
#define  _SERVICE_COMMUNICATE_H_

#include "System_DataPool.h"

#ifdef  __cplusplus
extern "C"{
#endif
/*------------------------------ System Handlers ------------------------------*/
extern TaskHandle_t CAN1SendPort_Handle;
extern TaskHandle_t CAN2SendPort_Handle;
extern TaskHandle_t CANReceivePort_Handle;
extern TaskHandle_t UartTransmitPort_Handle;
	
extern TaskHandle_t CAN1TxPort_Handle;
extern TaskHandle_t CAN2TxPort_Handle;
extern TaskHandle_t CAN1RxPort_Handle;
extern TaskHandle_t CAN2RxPort_Handle;
extern TaskHandle_t UsartRxPort_Handle;

/*------------------------------Function prototypes ---------------------------*/
uint32_t DR16_call_back(uint8_t* Recv_Data, uint16_t ReceiveLen);
uint32_t User_UART4_RxCpltCallback(uint8_t* Recv_Data, uint16_t ReceiveLen);
uint32_t GetViaionData(uint8_t* Recv_Data, uint16_t ReceiveLen);
void User_CAN1_RxCpltCallback(CAN_RxBuffer *CAN_RxMessage);
void User_CAN2_RxCpltCallback(CAN_RxBuffer *CAN_RxMessage);
void Service_Communication_Init(void);

#ifdef  __cplusplus
}
#endif
#endif  

/************************* COPYRIGHT SCUT-ROBOTLAB *****END OF FILE****************/

