/**@file            User_SPI.c
* @brief            The drivers support STM32F103
* @author           oyz
* @date             2024.10.22
* @version          1.0
* @copyright        Shenzhen DX-SMART Technology Co.Ltd
**********************************************************************************
*/
#include "User_spi.h"
#include "usart.h"
#include "delay.h"





void SW_SPI_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_7;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_0;
	GPIO_Init(GPIOB,&GPIO_InitStruct);
	
	
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_IPU;
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_6;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_1;
	GPIO_Init(GPIOB,&GPIO_InitStruct);
	
	SPI_CE_PIN=0;
	SPI_SCK_PIN=0;
	SPI_IRQ_PIN=1;
	SPI_CSN_PIN=1;
}

u8 SW_SPI_ReadWriteByte(u8 byte)
{
	u8 i;
	for(i=0;i<8; i++)
	{
		SPI_MOSI_PIN=((byte&0x80)>>7);//取byte最高一位，读取寄存器状态
		byte=(byte<<1);
		SPI_SCK_PIN=1;
		byte |=SPI_MISO_PIN;
		SPI_SCK_PIN=0;
	}
	return (byte);




}

