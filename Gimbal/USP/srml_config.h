/**
  ******************************************************************************
  * @file    srml_config_template.h
  * @brief   SRML configuration template file.
  *          This file should be copied to the application folder and renamed
  *          to srml_config.h.
  * @version 0.0.1
  ******************************************************************************
  * MOST IMPORTANTLY, this library is not open source for developers from other
  * schools' robot team currently. Plz make sure your code is under your supervision.
  *
  * Thank for @mannychen @HuanpengLu and other pioneers who have put forward such
  * architechure, and the endeavors of all developers.
  *
  * By downloading, copying, installing or using the software you agree to this license.
  * If you do not agree to this license, do not download, install,
  * copy or use the software.
  *
  *                          License Agreement
  *                For SCUT RobotLab Middleware Layer Library
  *
  * Copyright (c) 2019 - ~, SCUT RobotLab Development Team, all rights reserved.
  */

#ifndef __SRML_CONFIG_H__
#define __SRML_CONFIG_H__

/* ########################## Module Selection ############################## */
/**
  * @brief This is the list of modules to be used in the SRML.
  *        Change the value to 1 to use it.
  */

/* Drivers ----------------------------------------------------*/
#define USE_SRML_CAN                      1
#define USE_SRML_FLASH                    0
#define USE_SRML_I2C                      1
#define USE_SRML_SPI                      0
#define USE_SRML_TIMER                    1
#define USE_SRML_UART                     1

#define USE_SRML_BMX055                   0
#define USE_SRML_FATFS                    0
#define USE_SRML_W25Qx                    0
#define USE_SRML_MPU6050                  1
#define USE_SRML_DR16                     1
#define USE_SRML_MOTOR_AK80               0
#define USE_SRML_MOTOR_DJI                1
#define USE_SRML_REFEREE                  1
#define USE_SRML_VSEC                     0


/* Middlewares -----------------------------------------------*/
#define USE_SRML_FILTER                   1
#define USE_SRML_PID                      1

#define USE_SRML_CHASSIS                  0
#define USE_SRML_MOTOR_CTRL               0
#define USE_SRML_POW_CTRL                 0

#define USE_SRML_SERIAL_LINE_IP           0

#define USE_SRML_ASUWAVE                  0
#define USE_SRML_LIST                     0
#define USE_SRML_MYASSERT                 0
#define USE_SRML_SYSANALYSIS              0
#define USE_SRML_SYSLOG                   0

/* Includes ------------------------------------------------------------------*/
/**
  * @brief Include module's header file
  */

/* Drivers ----------------------------------------------------*/

/* Components header begin */
#if USE_SRML_CAN
  #include "Drivers/Components/drv_can.h"
#endif
#if USE_SRML_FLASH
  #include "Drivers/Components/drv_flash.h"
#endif
#if USE_SRML_I2C
  #include "Drivers/Components/drv_i2c.h"
#endif
#if USE_SRML_SPI
  #include "Drivers/Components/drv_spi.h"
#endif
#if USE_SRML_TIMER
  #include "Drivers/Components/drv_timer.h"
#endif
#if USE_SRML_UART
  #include "Drivers/Components/drv_uart.h"
#endif
/* Components header end */

/* Devices header begin */
#if USE_SRML_BMX055
  #include "Drivers/Devices/BMX055/BMX055_config.h"
#endif
#if USE_SRML_FATFS
  #include "Drivers/Devices/Flash/FATFS/diskio.h"
#endif
#if USE_SRML_W25Qx
  #include "Drivers/Devices/Flash/W25Qx.h"
#endif
#if USE_SRML_MPU6050
  #include "Drivers/Devices/MPU6050/mpu6050_config.h"
#endif
#if USE_SRML_DR16
  #include "Drivers/Devices/dr16.h"
#endif
#if USE_SRML_MOTOR_AK80
  #include "Drivers/Devices/motor_AK80.h"
#endif
#if USE_SRML_MOTOR_DJI
  #include "Drivers/Devices/motor_dji.h"
#endif
#if USE_SRML_REFEREE
  #include "Drivers/Devices/referee.h"
#endif
#if USE_SRML_VSEC
  #include "Drivers/Devices/VSEC.h"
#endif
/* Devices header end */


/* Middlewares -----------------------------------------------*/
/* Algorithms header begin */
#if USE_SRML_FILTER
  #include "Middlewares/Algorithm/filters.h"
#endif
#if USE_SRML_PID
  #include "Middlewares/Algorithm/PID.h"
#endif
/* Algorithms header end */

/* Modules header begin */
#if USE_SRML_CHASSIS
  #include "Middlewares/Module/chassis.h"
#endif
#if USE_SRML_MOTOR_CTRL
  #include "Middlewares/Module/motor_ctrl.h"
#endif
#if USE_SRML_POW_CTRL
  #include "Middlewares/Module/power_ctrl.h"
#endif
/* Modules header end */

/* Protocols header begin */
#if USE_SRML_SERIAL_LINE_IP
  #include "Middlewares/Protocol/serial_line_ip.h"
#endif
/* Protocols header end */

/* Utilities header begin */
#if USE_SRML_ASUWAVE
  #include "Middlewares/Utility/asuwave.h"
#endif
#if USE_SRML_LIST
  #include "Middlewares/Utility/linux_list.h"
#endif
#if USE_SRML_MYASSERT
  #include "Middlewares/Utility/my_assert.h"
#endif
#if USE_SRML_SYSANALYSIS
  #include "Middlewares/Utility/sys_analysis.h"
#endif
#if USE_SRML_SYSLOG
  #include "Middlewares/Utility/sys_log.h"
#endif
/* Utilities header end */

#endif /* __SRML_CONFIG_H__ */

/************************ COPYRIGHT(C) SCUT-ROBOTLAB **************************/
