/**
  ******************************************************************************
  * Copyright (c) 2019 - ~, SCUT-RobotLab Development Team
  * @file    PID.h
  * @author  BigeYoung 851756890@qq.com & M3chD09
  * @brief   PID controller sets. This file provides traditional PID controller 
  *			     and modern Fuzzy PID controller.
  ******************************************************************************
  * @attention
  * 
  * if you had modified this file, please make sure your code does not have any 
  * bugs, update the version Number, write dowm your name and the date. The most
  * important thing is make sure the users will have clear and definite under-
  * standing through your new brief.
  *
  * <h2><center>&copy; Copyright (c) 2019 - ~, SCUT-RobotLab Development Team.
  * All rights reserved.</center></h2>
  ******************************************************************************
  */
#pragma once

#ifdef __cplusplus
/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include <stddef.h>
#include <limits.h>
#include "filters.h"   /* LowPassFilter() */
/* Private macros ------------------------------------------------------------*/
typedef uint32_t (*SystemTick_Fun)(void);
/* Private type --------------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
class myPIDTimer
{
public:
    static uint8_t getMicroTick_regist(uint32_t (*getTick_fun)(void));
                                            /*<! Regist get time function */
protected:
    static SystemTick_Fun Get_SystemTick;   /*<! Pointer of function to get system tick */
    float dt;				                        /*!< Differentiation of real time*/
    uint32_t last_time; 	                  /*!< Last recorded real time from systick*/
    uint8_t UpdataTimeStamp(void);                                 
};

/** 
* @brief Class for open loop control.
*/
class OpenLoop
{
public:
    OpenLoop(float gain = 1.0f) : Gain(gain) {}

    float Adjust()
    {
        Out = Gain * Target;
        return Out;
    }
    float Target = 0, Current = 0;
    float Out = 0;

    float Gain = 1.0f;
};

/** 
* @brief Class for traditional PID control.
*/
class myPID : public myPIDTimer
{
public:
    myPID() {}
    myPID(float _Kp, float _Ki, float _Kd) : Kp(_Kp), Ki(_Ki), Kd(_Kd){}
    void SetPIDParam(float _Kp, float _Ki, float _Kd, float _I_Term_Max, float _Out_Max)
    {
      Kp = _Kp;
      Ki = _Ki;
      Kd = _Kd;
      I_Term_Max = _I_Term_Max;
      Out_Max = _Out_Max;
    };
    float Adjust();
    float Target = 0, Current = 0, Error = 0;
    float Out = 0;

    float Kp = 0, Ki = 0, Kd = 0;
    float I_Term_Max = 0;        /*<! I项限幅 */
    float Out_Max = 0;           /*<! 输出限幅 */
	
	float I_Term = 0;			/* 积分器输出 */
    float P_Term = 0;			/* 比例器输出 */
    float D_Term = 0;			/* 微分器输出 */

    float I_SeparThresh = 400;   /*!< 积分分离阈值，需为正数。fabs(error)大于该阈值取消积分作用。*/


    float VarSpeed_I_A = ULONG_MAX; /*!< 变速积分 A，需为正数。*/
    float VarSpeed_I_B = ULONG_MAX; /*!< 变速积分 B，需为正数， \n
                                     在 error<=B 的区间内，为普通积分效果， \n
                                     在 B<error<=A+B 的区间内，为变速积分效果， \n
                                     在 A+B<error 的区间内，不继续积分。*/

    float DeadZone = 0; 		    /*!< 死区，需为整数，fabs(error)小于DeadZone时，输出为0。 */

    LowPassFilter LowPass_error = LowPassFilter(1);
    LowPassFilter LowPass_d_err = LowPassFilter(1); /*!< 不完全微分。 */

    bool D_of_Current = false; /*!< 启用微分先行，文献中Current多译作Process Variable(PV)。 */

private:
    float pre_error = 0;
    float integral_e = 0;

    float pre_Current = 0;
};

/** 
* @brief Class for Fuzzy PID control.
*/
class FuzzyPID : public myPIDTimer
{
public:
    float Adjust();
    float Target = 0, Current = 0, Error;
    float Out = 0;

    float Table[3][3] = {
        /*				d_err_N, d_err_Z, d_err_P */
        /* error_N */ { -200,    -100,    0       },
        /* error_Z */ { -100,    0,       100     },
        /* error_P */ { 0,       100,     200     }
        };

    // N(负数, Negative)
    // Z(零, Zero)
    // P(正数, Positive)
    float error_N = -ULONG_MAX, error_P = ULONG_MAX;
    float d_err_N = -ULONG_MAX, d_err_P = ULONG_MAX;

    LowPassFilter LowPass_d_err = LowPassFilter(0);

private:
    float pre_error = 0;
    float Membership(float x, float Negative, float Positive, int POS);
    float Linear(float X, float X0, float X1)
    {
        return (X - X0) / (X1 - X0);
    }
};

/* Exported variables ---------------------------------------------------------*/
/* Exported function declarations ---------------------------------------------*/
#endif

/************************ COPYRIGHT(C) SCUT-ROBOTLAB **************************/
