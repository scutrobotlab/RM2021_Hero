/**
 ******************************************************************************
 * @file      asuwave.c
 * @author    M3chD09 m3chd09@protonmail.com
 * @brief     
 * @version   1.0
 * @date      6th Apr 2021
 ******************************************************************************
  ==============================================================================
                               How to use this Utility
  ==============================================================================
    [..]
      Initialize the asuwave using asuwave_init() function:
        Specify the UART to communicate with computer.
        Specify the function to obtain system tick,
          which can be xTaskGetTickCount if using FreeRTOS.
    [..]
      Register asuwave_callback() in UART callback function.
    [..]
      Call asuwave_subscribe() in a FreeRTOS task:
        A delay of 10ms is OK.
 ******************************************************************************
 */

#include "srml.h"

#if USE_SRML_ASUWAVE

/* Includes ------------------------------------------------------------------*/
#include "asuwave.h"
#include "string.h"

/* Private variables ---------------------------------------------------------*/
typedef uint32_t (*getTick_f)(void);
static getTick_f getTick;
static UART_HandleTypeDef *huart_x;

/**
 * @brief  board ID definition
 */
enum ASUWAVE_BOARD
{
  ASUWAVE_BOARD_1 = 0x01,
  ASUWAVE_BOARD_2,
  ASUWAVE_BOARD_3
};

/**
 * @brief  action information definition
 */
enum ASUWAVE_ACT
{
  ASUWAVE_ACT_SUBSCRIBE = 0x01,
  ASUWAVE_ACT_SUBSCRIBERETURN,
  ASUWAVE_ACT_UNSUBSCRIBE,
  ASUWAVE_ACT_UNSUBSCRIBERETURN,
  ASUWAVE_ACT_READ,
  ASUWAVE_ACT_READRETURN,
  ASUWAVE_ACT_WRITE,
  ASUWAVE_ACT_WRITERETURN
};

/**
 * @brief  error information definition
 */
enum ASUWAVE_ERROR
{
  ASUWAVE_ERROR_NOSUCHADDRREG = 0xf9,
  ASUWAVE_ERROR_FULLADDR,
  ASUWAVE_ERROR_NOSUCHDATANUM,
  ASUWAVE_ERROR_NOSUCHADDR,
  ASUWAVE_ERROR_NOSUCHACT,
  ASUWAVE_ERROR_NOSUCHBOARD
};

/** 
 * @brief asuwave data to receive structure definition
 */
typedef struct
{
  uint8_t board :8; /*!< The ID of the board to be operated.
   This parameter can be any value of @ref ASUWAVE_BOARD */

  uint8_t act :8; /*!< The action to be performed or the error information to be sent.
   This parameter can be any value of @ref ASUWAVE_ACT or @ref ASUWAVE_ERROR */

  uint8_t dataNum :8; /*!< Size amount of data to be operated.
   This parameter must be a number between Min_Data = 0 and Max_Data = 8. */

  uint32_t addr :32; /*!< The address of MCU to be operated.
   This parameter must be a number between Min_Data = 0x20000000 and Max_Data = 0x80000000. */

  uint64_t dataBuf :64; /*!< The data read or to be written.
   This parameter must be a variable of type uint64_t */

  uint8_t carriageReturn :8; /*!< The carriage return.
   This parameter must be '\n' */

} __attribute__((packed)) asuwave_rx_t;

/**
 * @brief asuwave data to transmit structure definition
 */
typedef struct
{
  uint8_t board :8; /*!< The ID of the board to be operated.
   This parameter can be any value of @ref ASUWAVE_BOARD */

  uint8_t act :8; /*!< The action to be performed or the error information to be sent.
   This parameter can be any value of @ref ASUWAVE_ACT or @ref ASUWAVE_ERROR */

  uint8_t dataNum :8; /*!< Size amount of data to be operated.
   This parameter must be a number between Min_Data = 0 and Max_Data = 8. */

  uint32_t addr :32; /*!< The address of MCU to be operated.
   This parameter must be a number between Min_Data = 0x20000000 and Max_Data = 0x80000000. */

  uint64_t dataBuf :64; /*!< The data read or to be written.
   This parameter must be a variable of type uint64_t */

  uint32_t tick :32; /*!< The tick count.
   This parameter must be a variable of type uint32_t */

  uint8_t carriageReturn :8; /*!< The carriage return.
   This parameter must be '\n' */

} __attribute__((packed)) asuwave_tx_t;

/** 
 * @brief asuwave data to receive union definition
 */
typedef union
{
  asuwave_rx_t body;
  uint8_t buff[16];
} asuwave_rxu_t;

/** 
 * @brief asuwave data to transmit union definition
 */
typedef union
{
  asuwave_tx_t body;
  uint8_t buff[20];
} asuwave_txu_t;

/* definition of the list of address to read */
#define MAX_ADDR_NUM 5
typedef struct
{
  uint8_t dataNum;
  uint32_t addr;
} list_addr_t;
static list_addr_t list_addr[MAX_ADDR_NUM];

/* function prototypes -------------------------------------------------------*/

/**
 * @brief  Register the address.
 * @param  asuwave_rxu: received asuwave data union.
 * @retval the index of the list_addr to register.
 */
int8_t addr_register(asuwave_rxu_t *asuwave_rxu)
{
  for (int i = 0; i < MAX_ADDR_NUM; i++)
  {
    /* Find the index of list_addr that is null */
    if (list_addr[i].dataNum == 0
        || list_addr[i].addr == asuwave_rxu->body.addr)
    {
      list_addr[i].dataNum = asuwave_rxu->body.dataNum;
      list_addr[i].addr = asuwave_rxu->body.addr;
      return i;
    }
  }
  return -1;
}

/**
 * @brief  Unregister the address.
 * @param  asuwave_rxu: received asuwave data union.
 * @retval the index of the list_addr to unregister.
 */
int8_t addr_unregister(asuwave_rxu_t *asuwave_rxu)
{
  for (int i = 0; i < MAX_ADDR_NUM; i++)
  {
    /* Find the index of list_addr */
    if (list_addr[i].addr == asuwave_rxu->body.addr)
    {
      list_addr[i].dataNum = 0;
      list_addr[i].addr = 0;
      return i;
    }
  }
  return -1;
}

/**
 * @brief  Send an error message via uart.
 * @param  err: the error information.
 * @param  asuwave_rxu: received asuwave data union.
 * @retval None
 */
void return_err(asuwave_rxu_t *asuwave_rxu, uint8_t err)
{
  /* Clear the data buffer to be sent */
  asuwave_txu_t asuwave_txu;
  memset((uint8_t*) &asuwave_txu.buff, 0,
      sizeof(asuwave_txu.buff));

  /* Prepare the data to send */
  asuwave_txu.body.board = asuwave_rxu->body.board;
  asuwave_txu.body.act = err;
  asuwave_txu.body.addr = asuwave_rxu->body.addr;
  asuwave_txu.body.dataNum = asuwave_rxu->body.dataNum;
  asuwave_txu.body.carriageReturn = '\n';

  /* Send the error message */
  HAL_UART_Transmit_DMA(huart_x, asuwave_txu.buff, 9);
}

/**
 * @brief  Subscribes the variable in flash memory of the given address.
 * @param  None
 * @retval None
 */
void asuwave_subscribe(void)
{
  /* definition of the data pack and length to send */
  static uint8_t tx_buff[MAX_ADDR_NUM * 20] = { 0 };
  static uint16_t tx_len = 0;
  static asuwave_txu_t asuwave_txu;

  /* Clear the data buffer to be sent */
  memset((uint8_t*) &asuwave_txu.buff, 0,
      sizeof(asuwave_txu.buff));
  memset((uint8_t*) &tx_buff, 0, sizeof(tx_buff));
  tx_len = 0;

  /* Prepare the data to send */
  asuwave_txu.body.board = ASUWAVE_BOARD_1;
  asuwave_txu.body.act = ASUWAVE_ACT_SUBSCRIBERETURN;
  asuwave_txu.body.carriageReturn = '\n';

  uint16_t dataNum;
  uint32_t addr;
  for (int i = 0; i < MAX_ADDR_NUM; i++)
  {
    if (list_addr[i].dataNum != 0)
    {
      addr = list_addr[i].addr;
      asuwave_txu.body.addr = list_addr[i].addr;
      /* Reads the variable in flash memory */
      for (dataNum = 0; dataNum < list_addr[i].dataNum; dataNum++)
      {
        *(asuwave_txu.buff + 7 + dataNum) = *(__IO uint8_t*) addr++;
      }
      asuwave_txu.body.dataNum = dataNum;
      asuwave_txu.body.tick = getTick();
      memcpy((uint8_t*) (tx_buff + tx_len),
          (uint8_t*) asuwave_txu.buff, 20);
      tx_len += 20;
    }
  }
  /* Send return data */
  HAL_UART_Transmit_DMA(huart_x, (uint8_t*) tx_buff, tx_len);
}

/**
 * @brief  Writes the given data buffer to the flash of the given address.
 * @param  asuwave_rxu: received asuwave data union.
 * @retval None
 */
void write_flash(asuwave_rxu_t *asuwave_rxu)
{
  /* Clear the data buffer to be sent */
  asuwave_txu_t asuwave_txu;
  memset((uint8_t*) &asuwave_txu.buff, 0,
      sizeof(asuwave_txu.buff));

  /* Prepare the data to send */
  asuwave_txu.body.board = asuwave_rxu->body.board;
  asuwave_txu.body.addr = asuwave_rxu->body.addr;
  asuwave_txu.body.dataNum = asuwave_rxu->body.dataNum;
  asuwave_txu.body.carriageReturn = '\n';

  /* Write data buffer */
  uint32_t TypeProgram = 0;
  uint8_t n = asuwave_rxu->body.dataNum;
  if (n > 8 || (n & (n - 1)))
  {
    return_err(asuwave_rxu, ASUWAVE_ERROR_NOSUCHDATANUM);
    return;
  }
  while (n >>= 1) TypeProgram++;

  if (HAL_FLASH_Unlock() == HAL_OK)
    if (HAL_FLASH_Program(TypeProgram, asuwave_rxu->body.addr,
        asuwave_rxu->body.dataBuf) == HAL_OK)
      if (HAL_FLASH_Lock() == HAL_OK)
        asuwave_txu.body.act = ASUWAVE_ACT_WRITERETURN;

  /* Send return data */
  HAL_UART_Transmit_DMA(huart_x, asuwave_txu.buff, 8);
}

/**
 * @brief  asuwave callback.
 * @param  data_buf: received buffer array.
 * @param  length: the length of array.
 * @retval None
 */
uint32_t asuwave_callback(uint8_t *data_buf, uint16_t length)
{
  asuwave_rxu_t asuwave_rxu;
  if (length != sizeof(asuwave_rxu.buff)) return 1;
  memcpy(&asuwave_rxu.buff, data_buf, length);

  /* Check if it is a valid board ID */
  if (asuwave_rxu.body.board == ASUWAVE_BOARD_1)
  {

    /* Check if it is a valid action information and execute */
    switch (asuwave_rxu.body.act)
    {
      case ASUWAVE_ACT_SUBSCRIBE:
        if (addr_register(&asuwave_rxu) == -1)
          return_err(&asuwave_rxu, ASUWAVE_ERROR_FULLADDR);
        break;
      case ASUWAVE_ACT_WRITE:
        write_flash(&asuwave_rxu);
        break;
      case ASUWAVE_ACT_UNSUBSCRIBE:
        if (addr_unregister(&asuwave_rxu) == -1)
          return_err(&asuwave_rxu, ASUWAVE_ERROR_NOSUCHADDRREG);
        break;
      default:
        return_err(&asuwave_rxu, ASUWAVE_ERROR_NOSUCHACT);
        return 1;
    }

  }
  else if (asuwave_rxu.body.board == ASUWAVE_BOARD_2
      || asuwave_rxu.body.board == ASUWAVE_BOARD_3)
  {
    //TODO
  }
  else
  {
    return_err(&asuwave_rxu, ASUWAVE_ERROR_NOSUCHBOARD);
  }
  return 0;
}

/**
 * @brief  Register uart device.
 * @param  *huart: pointer of uart IRQHandler.
 * @param  *f: function pointer to obtain system tick, which can be xTaskGetTickCount if using FreeRTOS
 * @retval None
 */
void asuwave_init(UART_HandleTypeDef *huart, uint32_t (*f)(void))
{
  huart_x = huart;
  getTick = f;
}

#endif /* USE_SRML_ASUWAVE */
