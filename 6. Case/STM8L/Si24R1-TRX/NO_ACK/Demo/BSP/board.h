/**@file  	    board.h
* @brief            Pin description corresponding to each peripheral(STM8L101F3P6)
* @author           oyz
* @date             2024.10.22
* @version          1.0
* @copyright        Shenzhen DX-SMART Technology Co.Ltd
**********************************************************************************
*/
#ifndef E01_STM8L101F3P6_BOARD_H
#define E01_STM8L101F3P6_BOARD_H

//LED
#define PORT_LED0       GPIOB
#define PIN_LED0        GPIO_Pin_0
#define PORT_LED1       GPIOD
#define PIN_LED1        GPIO_Pin_0

//SPI
#define PORT_SPI        GPIOB
#define PIN_SCLK        GPIO_Pin_5        
#define PIN_MOSI        GPIO_Pin_6
#define PIN_MISO        GPIO_Pin_7

//USART
#define PORT_USART      GPIOC
#define PIN_TXD         GPIO_Pin_3
#define PIN_RXD         GPIO_Pin_2

//nRF24L01P Ctrl
#define PORT_L01_CSN    GPIOB
#define PIN_L01_CSN     GPIO_Pin_4

#define PORT_L01_IRQ    GPIOA
#define PIN_L01_IRQ     GPIO_Pin_3

#define PORT_L01_CE     GPIOA
#define PIN_L01_CE      GPIO_Pin_2

#define KEY1_PORT  GPIOB
#define KEY1_PINS  GPIO_Pin_1   //µ×°å°´¼ü


#endif