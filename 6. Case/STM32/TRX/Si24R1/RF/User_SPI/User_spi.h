/**@file            User_SPI.h
* @brief            The drivers support STM32F103
* @author           oyz
* @date             2024.10.22
* @version          1.0
* @copyright        Shenzhen DX-SMART Technology Co.Ltd
**********************************************************************************
*/
#ifndef  __USER_SPI_H
#define  __USER_SPI_H
#include "stm32f10x.h"      

//SPI Òý½Å¶¨Òå
#define SPI_CSN_PIN         PAout(4)
#define SPI_CE_PIN			PBout(0)
#define SPI_IRQ_PIN			PBin(1)
#define SPI_SCK_PIN			PAout(5)
#define SPI_MISO_PIN		PAin(6)	
#define SPI_MOSI_PIN		PAout(7)

void SW_SPI_Init(void);

u8 SW_SPI_ReadWriteByte(u8 byte);


#endif