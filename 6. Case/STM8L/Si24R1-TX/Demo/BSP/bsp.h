/**@file  	    bsp.h
* @brief            The board drivers support(STM8L101F3P6)
* @author           oyz
* @date             2024.10.22
* @version          1.0
* @copyright        Shenzhen DX-SMART Technology Co.Ltd
**********************************************************************************
*/
#ifndef E01_STM8_BSP_H
#define E01_STM8_BSP_H

#include "RingBuffer.h"
#include "STM8l10x_conf.h"
#include "board.h"
#include "nRF24L01P.h"
#include "MyTypeDef.h"

typedef enum
{
  UART_TX, //corresponding to Uart_TxBuffer
  UART_RX, //corresponding to Uart_RxBuffer
}mode_t;

// LED操作函数，(ON)打开, (OFF)关闭，(TOG)翻转
#define LED0_ON()       GPIO_ResetBits(PORT_LED0, PIN_LED0)        
#define LED0_OFF()      GPIO_SetBits(PORT_LED0, PIN_LED0)
#define LED0_TOG()      GPIO_ToggleBits(PORT_LED0, PIN_LED0)

#define LED1_ON()       GPIO_ResetBits(PORT_LED1, PIN_LED1)        
#define LED1_OFF()      GPIO_SetBits(PORT_LED1, PIN_LED1)
#define LED1_TOG()      GPIO_ToggleBits(PORT_LED1, PIN_LED1)
/*
================================================================================
----------------------------The Function Declaration----------------------------
================================================================================
*/
void System_Initial(void);
void BSP_MCU_Initial(void);
void BSP_RF_Initial(void);
void BSP_SCLK_Initial(void);
void BSP_GPIO_Initial(void);
void BSP_TIM3_Initial(void);
void BSP_USART_Initial(void);
void BSP_USART_FIFO_Init(void);
void BSP_USART_ReadBuffer(mode_t mode,INT8U *buffer,INT16U size);
void BSP_USART_WriteBuffer(mode_t mode,INT8U *buffer,INT16U size);
void BSP_USART_WriteByte(mode_t mode,INT8U value);
INT8U BSP_USART_ReadByte(mode_t mode);
INT16U BSP_USART_GetValidSize(mode_t mode);
void BSP_USART_DiscardData(mode_t mode);
void BSP_SPI_Initial(void);
INT8U BSP_SPI_ExchangeByte(INT8U w_data);
#endif
