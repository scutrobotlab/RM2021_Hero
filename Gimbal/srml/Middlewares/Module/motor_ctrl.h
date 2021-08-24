/**
 * @file motor_ctrl.h
 * @author BigeYoung, M3chD09
 * @brief  电机控制库
 * @version 1.0
 * @date Jun 26, 2019
 */

#pragma once

#ifdef __cplusplus

#include "Drivers/Devices/motor_dji.h"
#include "Middlewares/Algorithm/PID.h"

class MotorCtrlBase
{
public:
    virtual ~MotorCtrlBase() {};
    virtual void setTarget(float target) = 0;
    virtual void incTarget(float target) = 0;
    virtual void Adjust() = 0;
};

template<class AnglePIDType>
class MotorAngleCtrl : public MotorCtrlBase
{
public:
    MotorAngleCtrl(MotorBase* const _motor) : motor(_motor) {}
    virtual ~MotorAngleCtrl() {}
    virtual void setTarget(float angle) override
    {
        AnglePID.Target = angle;
    }
    virtual void incTarget(float angle) override
    {
        AnglePID.Target += angle;
    }
    virtual void Adjust() override
    {
        AnglePID.Current = motor->getAngle();
        motor->Out = AnglePID.Adjust();
    }
    AnglePIDType AnglePID;
    MotorBase* const motor;
};


template<class SpeedPIDType>
class MotorSpeedCtrl : public MotorCtrlBase
{
public:
    MotorSpeedCtrl(MotorSpeed* const _motor) : motor(_motor) {}
    virtual ~MotorSpeedCtrl() {}
    virtual void setTarget(float speed) override
    {
        SpeedPID.Target = speed;
    }
    virtual void incTarget(float speed) override
    {
        SpeedPID.Target += speed;
    }
    virtual void Adjust() override
    {
        SpeedPID.Current = motor->getSpeed();
        motor->Out = SpeedPID.Adjust();
    }
    SpeedPIDType SpeedPID;
    MotorSpeed* const motor;
};


template<class AnglePIDType, class SpeedPIDType>
class MotorCascadeCtrl : public MotorCtrlBase
{
public:
    MotorCascadeCtrl(MotorSpeed* const _motor) : motor(_motor) {}
    virtual ~MotorCascadeCtrl() {}
    virtual void setTarget(float angle) override
    {
        AnglePID.Target = angle;
    }
    virtual void incTarget(float angle) override
    {
        AnglePID.Target += angle;
    }
    virtual void Adjust() override
    {
        SpeedPID.Current = motor->getSpeed();
        AnglePID.Current = motor->getAngle();
        SpeedPID.Target = AnglePID.Adjust();
        motor->Out = SpeedPID.Adjust();
    }
    AnglePIDType AnglePID;
    SpeedPIDType SpeedPID;

    MotorSpeed* const motor;
};

#endif
