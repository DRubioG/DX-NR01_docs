/**@file            app.c
* @brief
* @author           oyz
* @date             2024.10.22
* @version          1.0
* @copyright        Shenzhen DX-SMART Technology Co.Ltd
**********************************************************************************
*/

#include "app.h"

#define RF_PAYLOAD_MAX_SIZE  32
static INT16U _time_ticks = 0;
static INT8U Subcontract_Timeout = 0;
static INT8U RF_SendBuffer[32] = {0};
void APP_RecieveHandler(void);
void APP_TransmitHandler(void);
/*!
================================================================================
------------------------------------Functions-----------------------------------
================================================================================
*/
/*!
 *  @brief        Timer 1ms Interrupt CallBack
 *  @note
*/
void TIM3_1ms_Interrupt_Cb(void)
{
    _time_ticks++;
    if(Subcontract_Timeout != 0)
    {
        Subcontract_Timeout--;
    }
}
/*!
 *  @brief         Uart Rx Interrupt handler
 *  @note
*/
void Uart_Rx_interrupt_Cb(void)
{
    BSP_USART_WriteByte(UART_RX, USART_ReceiveData8());
    Subcontract_Timeout = 5;
}
/*!
 *  @brief         Uart Tx Interrupt handler
 *  @note
*/
void Uart_Tx_interrupt_Cb(void)
{
    INT8U value;
    if(BSP_USART_GetValidSize(UART_TX))
    {
        value = BSP_USART_ReadByte(UART_TX);
        USART_SendData8(value);
    }
    else
    {
        USART_ITConfig(USART_IT_TC, ENABLE);
        USART_ITConfig(USART_IT_TXE, DISABLE);
        if(USART_GetFlagStatus(USART_FLAG_TC))
        {
            USART_ITConfig(USART_IT_TC, DISABLE);
        }
        LED1_OFF();
    }
}
/*!
 *  @brief        Application entry
 *  @note
*/
void APP_Process(void)
{
    for(;;)
    {
        APP_RecieveHandler();
        APP_TransmitHandler();
    }
}
/*!
 *  @brief        Check whether the UART recieve data and subcontract it
 *  @param        None
 *  @return       the length recieved from uart after subcontracting
 *  @note
*/
INT8U Uart_RecieveLength(void)
{
    INT8U i, j = RF_PAYLOAD_MAX_SIZE;
    i = BSP_USART_GetValidSize(UART_RX);
    if(i >= j)
    {
        return j;
    }
    if(Subcontract_Timeout == 0)
    {
        return i;
    }
    return 0;
}
/*!
 *  @brief        Switch the current RF mode to RX
 *  @param        None
 *  @return       None
 *  @note
*/
void APP_SwitchToRx(void)
{
    L01_SetCE(CE_LOW);
    L01_SetPowerUp();
    L01_SetTRMode(RX_MODE);
    L01_SetDataRate(DRATE_250K);
    L01_WriteHoppingPoint(2);
    L01_FlushRX();
    L01_FlushTX();
    L01_ClearIRQ(IRQ_ALL);
    L01_SetCE(CE_HIGH);
}
/*!
 *  @brief        Switch the current RF mode to TX
 *  @param        None
 *  @return       None
 *  @note
*/
void APP_SwitchToTx(void)
{
    L01_SetCE(CE_LOW);
    L01_SetPowerUp();
    L01_SetTRMode(TX_MODE);
    L01_SetDataRate(DRATE_250K);
    L01_WriteHoppingPoint(2);
    L01_FlushRX();
    L01_FlushTX();
    L01_ClearIRQ(IRQ_ALL);
}


/*!
 *  @brief        Transmit handler for transmit mode
 *  @param        None
 *  @return       None
 *  @note
*/
void APP_TransmitHandler(void)
{
    INT8U len,IRQ_flag,ack_rx[32]={0};
    if((len = Uart_RecieveLength()) != 0)
    {
        LED0_ON();
        BSP_USART_ReadBuffer(UART_RX, RF_SendBuffer, len);       
        APP_SwitchToTx();
        L01_WriteTXPayload_Ack(RF_SendBuffer, len);
        L01_SetCE(CE_HIGH);
        while(GET_L01_IRQ() != 0);
        IRQ_flag=L01_ReadIRQSource();//读取中断标志位
        //BSP_USART_WriteBuffer(UART_TX, &IRQ_flag,1);
        if(IRQ_flag&0x20)//判断中断是否为发送完成中断
        {
           
          len=L01_ReadRXPayload(ack_rx);//读取应答数据及长度
          BSP_USART_WriteBuffer(UART_TX, &len, 1);
         BSP_USART_WriteBuffer(UART_TX, ack_rx, len);
          USART_ITConfig(USART_IT_TXE, ENABLE);
        }  
        if(IRQ_flag&0x10)//判断是否为发送超时中断
        {
          ack_rx[0]=0xFF;
          len=1;
          
        
          BSP_USART_WriteBuffer(UART_TX, ack_rx, len);
          USART_ITConfig(USART_IT_TXE, ENABLE);
        }
        L01_FlushTX();
        L01_ClearIRQ(IRQ_ALL);
        APP_SwitchToRx();
        LED0_OFF();
    }
}
/*!
 *  @brief        Recieve handler for transmit mode and Returns the accepted quantity
 *  @param        None
 *  @return       None
 *  @note:       从手册上看，需要在发送机发送之前写入应答，接收机接受到数据后，会自动发生中断
 *               并将写好的带负载ACK数据返回给发送机；这里返回的即时接收数据量（最大32字节），
 *               故写在接收中断之后。
*/
void APP_RecieveHandler(void)
{
    INT8U len, rcv_buffer[32];
    
    if(GET_L01_IRQ() == 0)
    {
        if(L01_ReadIRQSource() & (1 << RX_DR))//detect RF module recieve interrupt
        {
            if((len = L01_ReadRXPayload(rcv_buffer)) != 0)
            {
                LED1_ON();
                L01_FlushTX();//清空rf的发送fifo
                
                L01_WriteRXPayload_InAck(rcv_buffer,len);//写入应答数据,写入收到的数据量
                BSP_USART_WriteBuffer(UART_TX, rcv_buffer, len);
                USART_ITConfig(USART_IT_TXE, ENABLE);                
            }
        }
        L01_FlushRX();
        L01_ClearIRQ(IRQ_ALL);
    }
}
