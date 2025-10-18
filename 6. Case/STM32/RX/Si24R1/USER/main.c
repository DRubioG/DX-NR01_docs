/**@file            main.c
* @brief            Main program entry
* @author           oyz
* @date             2024.10.22
* @version          1.0
* @copyright        Shenzhen DX-SMART Technology Co.Ltd
**********************************************************************************
*/
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "stm32f10x.h"    
#include "si24r1.h"
#include "Timer.h"
#include "stm32f10x_gpio.h"
#include "LED.h"
#include "User_spi.h"

float length;
volatile u32 data=0;
int main(void)
{
	u8 u[32];
	u32 data=0;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 

	delay_init();
	
	uart_init(115200);//´®¿Ú1³õÊ¼»¯
	LED_Init();
	RF_RX_Init();
	
		
  while(1)	
	{
		
		if(Si24R1_RxPacket(u) ==0)
		{
			LED1_ON();
			printf("%s",u);

			
			
	
		}
		
	
	}
}
