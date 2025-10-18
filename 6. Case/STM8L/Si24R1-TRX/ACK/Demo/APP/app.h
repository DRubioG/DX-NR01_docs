/**@file  	    app.h
* @brief            
* @author           oyz
* @date             2024.10.22
* @version          1.0
* @copyright        Shenzhen DX-SMART Technology Co.Ltd
**********************************************************************************
*/
#ifndef APP_H
#define APP_H

#include "bsp.h"
#include "MyTypeDef.h"

/*
================================================================================
----------------------------The Function Declaration----------------------------
================================================================================
*/
void APP_Process(void);
void TIM3_1ms_Interrupt_Cb(void);
void Uart_Rx_interrupt_Cb(void);
void Uart_Tx_interrupt_Cb(void);
void APP_SwitchToRx(void);
void APP_SwitchToTx(void);
#endif
