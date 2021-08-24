/**
  ******************************************************************************
  * Copyright (c) 2019 - ~, SCUT-RobotLab Development Team
  * @file    motor_dji.h
  * @author  BigeYoung 851756890@qq.com & M3chD09
  * @brief   Code for Motor driver, Including almost every motor model that used 
  *			     in robomaster(Since robomaster 2019).
  * @date    2019-06-26
  * @version 1.2
  *
  ==============================================================================
                          How to use this library 
  ==============================================================================
    @warning
      - Standard C++11 required!
      - SRML Package(drv_can) required!
  ******************************************************************************
  * @attention
  * 
  * if you had modified this file, please make sure your code does not have many 
  * bugs, update the version Number, write dowm your name and the date, the most
  * important is make sure the users will have clear and definite understanding 
  * through your new brief.
  *
  * <h2><center>&copy; Copyright (c) 2019 - ~, SCUT-RobotLab Development Team.
  * All rights reserved.</center></h2>
  ******************************************************************************
  */
#pragma once

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include "Drivers/Components/drv_can.h"
#include "shit_mountain.h"
#ifdef __cplusplus
/* Private macros ------------------------------------------------------------*/
/* Private type --------------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
struct Motor_CAN_COB
{
    CAN_COB Low;
    CAN_COB High;
};
/* Exported variables --------------------------------------------------------*/
/* Exported function declarations --------------------------------------------*/
/**
  @brief  Base class for motors.
*/
class MotorBase
{
public:
    MotorBase(uint8_t id) : ID(id) {}
    virtual ~MotorBase() {}
    
    virtual void update(uint8_t can_rx_data[]) = 0;
    virtual bool CheckID(uint32_t StdID) const { return StdID == this->REC_ID_INIT() + (uint32_t)ID; }
    float getAngle() const { return angle; }

    const uint8_t ID = 0;
	
    float Out = 0; /*!< Output ampere value that sent to ESC */

protected:
    float angle = 0, encoder_offset = 0, last_encoder = 0;
    bool encoder_is_init = false;

    virtual uint32_t REC_ID_INIT() const { return 0; };

    void update_angle(uint8_t can_rx_data[])
    {
      encoder = (uint16_t)(can_rx_data[0] << 8 | can_rx_data[1]);
      if(encoder_is_init)
      {
        if (this->encoder - this->last_encoder > 4096)
            this->round_cnt--;
        else if (this->encoder - this->last_encoder < -4096)
            this->round_cnt++;
      }
      else
      {
        encoder_offset = encoder;
        encoder_is_init = true;
      }
      this->last_encoder = this->encoder;
      int32_t total_encoder = round_cnt * 8192 + encoder - encoder_offset;
      angle = total_encoder / ENCODER_ANGLE_RATIO();
    }

private:
    uint16_t encoder = 0;
    int32_t round_cnt = 0;
    virtual int16_t ENCODER_MAX() const { return 8192; }
    virtual float ENCODER_ANGLE_RATIO() const { return 8192.0f / 360.0f; }
    virtual float MAX_CURRENT() const { return 65535; }
};


class MotorSpeed : public MotorBase
{
public:
    MotorSpeed(uint8_t id) : MotorBase(id) {}
    virtual ~MotorSpeed(){};
    virtual float getSpeed() const { return this->speed; }
    virtual void update(uint8_t can_rx_data[]) = 0;

protected:
    int16_t speed = 0;
    void update_speed(uint8_t can_rx_data[])
    {
      this->speed = (int16_t)(can_rx_data[2] << 8 | can_rx_data[3]);
    }
};

/**
  @brief  Motor that used ESC 820R.
*/
class Motor_820R : public MotorSpeed
{
public:
    virtual uint32_t REC_ID_INIT() const { return 0x200; }
    virtual uint32_t SEND_ID_LOW() const { return 0x200; }
    virtual uint32_t SEND_ID_HIGH() const { return 0x1ff; }
    virtual float MAX_CURRENT() const { return 16384; }
    Motor_820R(uint8_t id) : MotorSpeed(id) {}

    virtual void update(uint8_t can_rx_data[]) override
    {
        this->update_angle(can_rx_data);
        this->update_speed(can_rx_data);
    }
};

/**
  @brief  GM3510.
*/
class Motor_GM3510 : public MotorBase
{
public:
    virtual uint32_t REC_ID_INIT() const { return 0x204; }
    virtual uint32_t SEND_ID_LOW() const { return 0x1ff; }
    virtual uint32_t SEND_ID_HIGH() const { return 0; }
    virtual float MAX_CURRENT() const { return 29000; }
    Motor_GM3510(uint8_t id) : MotorBase(id) {}

    int16_t getTorque() const { return torque; }

    virtual void update(uint8_t can_rx_data[]) override
    {
        this->update_angle(can_rx_data);
        torque = (int16_t)(can_rx_data[2] << 8 | can_rx_data[3]);
    }

protected:
    int16_t torque = 0;
};

/**
  @brief  Motor 6623.
*/
class Motor_6623 : public MotorBase
{
public:
    virtual uint32_t REC_ID_INIT() const { return 0x204; }
    virtual uint32_t SEND_ID_LOW() const { return 0x1ff; }
    virtual uint32_t SEND_ID_HIGH() const { return 0x2ff; }
    virtual float MAX_CURRENT() const { return 5000; }
    void setEncoderOffset(uint16_t offset) { 
      this->encoder_offset = offset;
      this->last_encoder   = offset;
      this->encoder_is_init = true;
    }
    enum
    {
        Yaw = 1,
        Pitch,
        Roll,
        Resv,
        Ex1,
        Ex2,
        Ex3,
        Ex4
    };
    Motor_6623(uint8_t id) : MotorBase(id) {}

    int16_t getTorque() const { return torque; }

    virtual void update(uint8_t can_rx_data[]) override
    {
        this->update_angle(can_rx_data);
        torque = (int16_t)(can_rx_data[2] << 8 | can_rx_data[3]);
    }

protected:
    int16_t torque = 0;
};

/**
  @brief  Motor that used ESC C610.
*/
class Motor_C610 : public MotorSpeed
{
public:
    virtual uint32_t REC_ID_INIT() const { return 0x200; }
    virtual uint32_t SEND_ID_LOW() const { return 0x200; }
    virtual uint32_t SEND_ID_HIGH() const { return 0x1ff; }
    virtual float MAX_CURRENT() const { return 10000; }
    Motor_C610(uint8_t id) : MotorSpeed(id) {}
    virtual void update(uint8_t can_rx_data[]) override
    {
        this->update_angle(can_rx_data);
        this->update_speed(can_rx_data);
        this->torque = (int16_t)(can_rx_data[4] << 8 | can_rx_data[5]);
    }
    int16_t getTorque() const { return torque; }

protected:
    int16_t torque = 0;
};

/**
  @brief  Motor that used ESC C620.
*/
class Motor_C620 : public MotorSpeed
{
public:
    virtual uint32_t REC_ID_INIT() const { return 0x200; }
    virtual uint32_t SEND_ID_LOW() const { return 0x200; }
    virtual uint32_t SEND_ID_HIGH() const { return 0x1ff; }
    virtual float MAX_CURRENT() const { return 16384; }
    Motor_C620(uint8_t id) : MotorSpeed(id) {}
    uint8_t getTempature() const { return temperature; }
    int16_t getGivenCurrent() const { return givenCurrent; }

    virtual void update(uint8_t can_rx_data[])
    {
        this->update_angle(can_rx_data);
        this->update_speed(can_rx_data);
        givenCurrent = (int16_t)(can_rx_data[4] << 8 | can_rx_data[5]);
        temperature = can_rx_data[6];
    }

protected:
    uint8_t temperature = 0;
    int16_t givenCurrent = 0;
};

/**
  @brief  GM6020.
*/
class Motor_GM6020 : public MotorSpeed
{
public:
    virtual uint32_t REC_ID_INIT() const { return 0x204; }
    virtual uint32_t SEND_ID_LOW() const { return 0x1ff; }
    virtual uint32_t SEND_ID_HIGH() const { return 0x2ff; }
    virtual float MAX_CURRENT() const { return 30000; }
    void setEncoderOffset(uint16_t offset) { 
        this->encoder_offset = offset;
        this->last_encoder   = offset;
        this->encoder_is_init = true;
    }
    Motor_GM6020(uint8_t id) : MotorSpeed(id) {}
    virtual void update(uint8_t can_rx_data[])
    {
        this->update_angle(can_rx_data);
        this->update_speed(can_rx_data);
        this->givenCurrent = (int16_t)(can_rx_data[4] << 8 | can_rx_data[5]);
        this->temperature = can_rx_data[6];
    }

protected:
    uint8_t temperature = 0;
    int16_t givenCurrent = 0;
};

/**
  Send CAN communication object.
*/
template <class MotorType, int N>
void MotorMsgSend(CAN_HandleTypeDef *hcan, MotorType (&motors)[N])
{
    uint8_t TxDataLow[8] = {0};
    uint8_t TxDataHigh[8] = {0};
    int16_t constrain_Out = 0;
    bool low = false;
    bool high = false;
    for (int i = 0; i < N; i++)
    {
    	motors[i].Out = shit::constrain(motors[i].Out, -motors[0].MAX_CURRENT(), motors[0].MAX_CURRENT());
        constrain_Out = motors[i].Out;
        if (motors[i].ID <= 4 && motors[i].ID > 0)
        {
            low = true;
            TxDataLow[motors[i].ID * 2 - 2] = (constrain_Out >> 8) & 0xff;
            TxDataLow[motors[i].ID * 2 - 1] = constrain_Out & 0xff;
        }
        else if (motors[i].ID <= 8 && motors[i].ID > 4)
        {
            high = true;
            TxDataHigh[motors[i].ID * 2 - 10] = (constrain_Out >> 8) & 0xff;
            TxDataHigh[motors[i].ID * 2 - 9] = constrain_Out & 0xff;
        }
    }

    if (low)
        CANx_SendData(hcan, motors[0].SEND_ID_LOW(), TxDataLow, 8);

    if (high)
        CANx_SendData(hcan, motors[0].SEND_ID_HIGH(), TxDataHigh, 8);
}

template <class MotorType>
void MotorMsgSend(CAN_HandleTypeDef *hcan, MotorType &motor)
{
    MotorType motor_arr[1] = {motor};
    MotorMsgSend(hcan, motor_arr);
}

template <class MotorType, int N>
Motor_CAN_COB &MotorMsgPack(Motor_CAN_COB &motor_msg, MotorType (&motors)[N])
{
    for (int i = 0; i < N; i++)
    {
        motor_msg = MotorMsgPack(motor_msg, motors[i]);
    }
    return motor_msg;
}

template <class MotorType, class... MotorTypes>
Motor_CAN_COB &MotorMsgPack(Motor_CAN_COB &motor_msg, MotorType &motor, MotorTypes&... motors)
{
    motor_msg = MotorMsgPack(motor_msg, motor);
    return MotorMsgPack(motor_msg, motors...);
}

template <class MotorType>
Motor_CAN_COB &MotorMsgPack(Motor_CAN_COB &motor_msg, MotorType &motor)
{
    motor.Out = shit::constrain(motor.Out, -motor.MAX_CURRENT(), motor.MAX_CURRENT());
    int16_t constrain_Out = motor.Out;
    if (motor.ID <= 4 && motor.ID > 0)
    {
        if (motor_msg.Low.ID == 0)
        {
            motor_msg.Low.DLC = 8;
            motor_msg.Low.ID = motor.SEND_ID_LOW();
        }
        else if (motor_msg.Low.ID != motor.SEND_ID_LOW())
        {
            motor_msg.Low = CAN_COB{};
            return motor_msg;
        }
        motor_msg.Low.Data[motor.ID * 2 - 2] = (constrain_Out >> 8) & 0xff;
        motor_msg.Low.Data[motor.ID * 2 - 1] = constrain_Out & 0xff;
    }
    else if (motor.ID <= 8 && motor.ID > 4)
    {
        if (motor_msg.High.ID == 0)
        {
            motor_msg.High.DLC = 8;
            motor_msg.High.ID = motor.SEND_ID_HIGH();
        }
        else if (motor_msg.High.ID != motor.SEND_ID_HIGH())
        {
            motor_msg.High = CAN_COB{};
            return motor_msg;
        }
        motor_msg.High.Data[motor.ID * 2 - 10] = (constrain_Out >> 8) & 0xff;
        motor_msg.High.Data[motor.ID * 2 - 9] = constrain_Out & 0xff;
    }
    return motor_msg;
}

#endif
/************************ COPYRIGHT(C) SCUT-ROBOTLAB **************************/
