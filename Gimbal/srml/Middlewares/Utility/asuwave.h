/**
 ******************************************************************************
 * @file      asuwave.h
 * @author    M3chD09 m3chd09@protonmail.com
 * @brief     Header file of asuwave.c
 * @version   1.0
 * @date      6th Apr 2021
 ******************************************************************************

 ******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _ASUWAVE_H_
#define _ASUWAVE_H_

/* Includes ------------------------------------------------------------------*/
#if defined(USE_HAL_DRIVER)
  #if defined(STM32F405xx) || defined(STM32F407xx)
    #include <stm32f4xx_hal.h>
  #endif
  #if defined(STM32F103xx)
    #include <stm32f1xx_hal.h>
  #endif
  #if defined(STM32H750xx)
    #include <stm32h7xx_hal.h>
  #endif
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* Exported function declarations --------------------------------------------*/
void asuwave_init(UART_HandleTypeDef *huart, uint32_t (*f)(void));
uint32_t asuwave_callback(uint8_t *data_buf, uint16_t length);
void asuwave_subscribe(void);

#ifdef __cplusplus
}
#endif

#endif /* _ASUWAVE_H_ */
