/**@file            main.c
* @brief            Main program entry
* @author           oyz
* @date             2024.10.22
* @version          1.0
* @copyright        Shenzhen DX-SMART Technology Co.Ltd
**********************************************************************************
*/
#include "Si24R1.h"
#include "uart.h"





/*******************************************************************************
* 函 数 名       : main
* 函数功能		 : 主函数
* 输    入       : 无
* 输    出    	 : 无
*******************************************************************************/
void main()
{	
	
	unsigned char u[32]={"Simple data verification\r\n"};
	UART_Init();
	RF_RX_Init();
	while(1)
	{
		
		
		Si24R1_TxPacket(u);
	

		
	
	}
}
