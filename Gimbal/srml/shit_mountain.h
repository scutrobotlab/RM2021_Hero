/**
  ******************************************************************************
  * Copyright (c) 2019 - ~, SCUT-RobotLab Development Team
  * @file    shit_mountain.h
  * @author  M3chD09 rinngo17@foxmail.com
  * @brief   Library full of shit.
  * @date    2021-04-05
  * @version 1.0
  *
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

#ifndef _SHIT_MOUNTAIN_H_
#define _SHIT_MOUNTAIN_H_

#ifdef  __cplusplus
/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
/* Private macros ------------------------------------------------------------*/
/* Private type --------------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/

namespace shit
{
/* Exported variables --------------------------------------------------------*/

/* Exported function declarations --------------------------------------------*/
template<class T>
T constrain(T input,T min,T max)
{
  if (input <= min)
    return min;
  else if(input >= max)
    return max;
  return input;
}

uint16_t get_gpio_pin_num(uint16_t GPIO_Pin);
}

#endif /* __cplusplus */

#endif /* _SHIT_MOUNTAIN_H_ */

/************************ COPYRIGHT(C) SCUT-ROBOTLAB **************************/
