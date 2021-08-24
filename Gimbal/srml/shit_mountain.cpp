/**
  ******************************************************************************
  * Copyright (c) 2019 - ~, SCUT-RobotLab Development Team
  * @file    shit_mountain.cpp
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

/* Includes ------------------------------------------------------------------*/
#include "shit_mountain.h"

using namespace shit;
/* Private define ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private type --------------------------------------------------------------*/
/* Private function declarations ---------------------------------------------*/
/* Function prototypes -------------------------------------------------------*/

/**
  * @brief  Get the number of the specified input port pin
  * @param  GPIO_Pin specifies the port number to get.
  *         This parameter can be GPIO_PIN_x where x can be (0..15).
  * @retval The x of GPIO_PIN_x
  */
uint16_t shit::get_gpio_pin_num(uint16_t GPIO_Pin)
{
  uint16_t x = 0;
  while (GPIO_Pin >>= 1)x++;
  return x;
}
