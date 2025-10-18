/**@file            bsp.c
* @brief            The drivers support E06-TB01 (STM8L101F3P6)
* @author           oyz
* @date             2024.10.22
* @version          1.0
* @copyright        Shenzhen DX-SMART Technology Co.Ltd
**********************************************************************************
*/

#include "bsp.h"
#include "app.h"
/*About Cycle FIFO */
INT8U Uart_TxBuffer[BUFFER_MAX_SIZE], Uart_RxBuffer[BUFFER_MAX_SIZE];
static _fifo_t fifo_tx;
static _fifo_t fifo_rx;
/*!
================================================================================
------------------------------------Functions-----------------------------------
================================================================================
*/
/*!
 *  @brief        Initialize the System
 *  @note
*/
void System_Initial(void)
{
    BSP_MCU_Initial();
    BSP_RF_Initial();
}
/*!
 *  @brief        Initialize the MCU
 *  @note
*/
void BSP_MCU_Initial(void)
{
    BSP_SCLK_Initial();
    BSP_GPIO_Initial();
    BSP_TIM3_Initial();
    BSP_USART_Initial();
    BSP_USART_FIFO_Init();
    BSP_USART_DiscardData(UART_TX);
    BSP_USART_DiscardData(UART_RX);
    BSP_SPI_Initial();
    enableInterrupts();
}
/*!
 *  @brief        Initialize the RF
 *  @note
*/
void BSP_RF_Initial(void)
{
    L01_Init();
    APP_SwitchToRx();
}
/*!
 *  @brief        Initialize the SystemClock
 *  @note         SystemClock  = 16M
*/
void BSP_SCLK_Initial(void)
{
    CLK_MasterPrescalerConfig(CLK_MasterPrescaler_HSIDiv1);
}
/*!
 *  @brief        Initialize the GPIO
 *  @note
*/
void BSP_GPIO_Initial(void)
{
    //LED0
    GPIO_Init(PORT_LED0, PIN_LED0, GPIO_Mode_Out_PP_High_Slow);
    GPIO_SetBits(PORT_LED0, PIN_LED0);
    //LED1
    GPIO_Init(PORT_LED1, PIN_LED1, GPIO_Mode_Out_PP_High_Slow);
    GPIO_SetBits(PORT_LED1, PIN_LED1);
    /*nRF24L01P Ctrl*/
    //IRQ
    GPIO_Init(PORT_L01_IRQ, PIN_L01_IRQ, GPIO_Mode_In_PU_No_IT);
    //CE
    GPIO_Init(PORT_L01_CE, PIN_L01_CE, GPIO_Mode_Out_PP_High_Slow);
    GPIO_SetBits(PORT_L01_CE, PIN_L01_CE);
    //CSN
    GPIO_Init(PORT_L01_CSN, PIN_L01_CSN, GPIO_Mode_Out_PP_High_Slow);
    GPIO_SetBits(PORT_L01_CSN, PIN_L01_CSN);
    
     GPIO_Init(KEY1_PORT, KEY1_PINS, GPIO_Mode_In_PU_No_IT);//初始化按键，上拉输入，
}
/*!
 *  @brief        Initialize the Timer
 *  @note         Timer Interval:1ms
*/
void BSP_TIM3_Initial(void)
{
    TIM3_DeInit();
    CLK_PeripheralClockConfig(CLK_Peripheral_TIM3, ENABLE);
    TIM3_TimeBaseInit(TIM3_Prescaler_16, TIM3_CounterMode_Up, 1000);
    TIM3_ITConfig(TIM3_IT_Update, ENABLE);
    TIM3_Cmd(ENABLE);
}
/*!
 *  @brief        Initialize the USART
 *  @note
*/
void BSP_USART_Initial(void)
{
    GPIO_Init(PORT_USART, PIN_RXD, GPIO_Mode_In_FL_No_IT);      // RXD
    GPIO_Init(PORT_USART, PIN_TXD, GPIO_Mode_Out_OD_HiZ_Fast);  // TXD
    CLK_PeripheralClockConfig(CLK_Peripheral_USART, ENABLE);
    USART_Init(115200, USART_WordLength_8D, USART_StopBits_1,
               USART_Parity_No, (USART_Mode_TypeDef)(USART_Mode_Rx | USART_Mode_Tx));
    USART_ITConfig(USART_IT_RXNE, ENABLE);
    USART_ClearITPendingBit();
    USART_Cmd(ENABLE);
}
/*!
 *  @brief        Register the UART Cycle FIFO：Uart_TxBuffer,Uart_RxBuffer
 *  @note
*/
void BSP_USART_FIFO_Init(void)
{
    fifo_register(&fifo_tx, Uart_TxBuffer, BUFFER_MAX_SIZE, NULL, NULL);
    fifo_register(&fifo_rx, Uart_RxBuffer, BUFFER_MAX_SIZE, NULL, NULL);
}
/*!
 *  @brief        Read from the UART FIFO：Uart_TxBuffer,Uart_RxBuffer
 *  @param        mode：UART_TX->Uart_TxBuffer,UART_RX->Uart_RxBuffer
 *  @param        buffer:the buffer stores the value read from the FIFO
 *  @param        size: the size to be read
 *  @return       None
 *  @note
*/
void BSP_USART_ReadBuffer(mode_t mode, INT8U* buffer, INT16U size)
{
    if(mode == UART_TX)
    {
        fifo_read(&fifo_tx, buffer, size);
    }
    else
    {
        fifo_read(&fifo_rx, buffer, size);
    }
}
/*!
 *  @brief        Write to the UART FIFO：Uart_TxBuffer,Uart_RxBuffer
 *  @param        mode：UART_TX->Uart_TxBuffer,UART_RX->Uart_RxBuffer
 *  @param        buffer：the buffer to be written
 *  @param        size：the size to be written
 *  @return       None
 *  @note
*/
void BSP_USART_WriteBuffer(mode_t mode, INT8U* buffer, INT16U size)
{
    if(mode == UART_TX)
    {
        fifo_write(&fifo_tx, buffer, size);
    }
    else
    {
        fifo_write(&fifo_rx, buffer, size);
    }
}
/*!
 *  @brief        Write a byte to the UART FIFO：Uart_TxBuffer,Uart_RxBuffer
 *  @param        mode：UART_TX->Uart_TxBuffer,UART_RX->Uart_RxBuffer
 *  @param        value：the value to be written
 *  @return
 *  @note
*/
void BSP_USART_WriteByte(mode_t mode, INT8U value)
{
    if(mode == UART_TX)
    {
        fifo_write(&fifo_tx, &value, 1);
    }
    else
    {
        fifo_write(&fifo_rx, &value, 1);
    }
}
/*!
 *  @brief        Read a byte from the UART FIFO：Uart_TxBuffer,Uart_RxBuffer
 *  @param        mode：UART_TX->Uart_TxBuffer,UART_RX->Uart_RxBuffer
 *  @return
 *  @note
*/
INT8U BSP_USART_ReadByte(mode_t mode)
{
    INT8U value;
    if(mode == UART_TX)
    {
        fifo_read(&fifo_tx, &value, 1);
    }
    else
    {
        fifo_read(&fifo_rx, &value, 1);
    }
    return value;
}
/*!
 *  @brief        Get the UART FIFO's valid size：Uart_TxBuffer,Uart_RxBuffer
 *  @param        mode：UART_TX->Uart_TxBuffer,UART_RX->Uart_RxBuffer
 *  @return       valid_size
 *  @note
*/
INT16U BSP_USART_GetValidSize(mode_t mode)
{
    INT16U valid_size;
    if(mode == UART_TX)
    {
        valid_size = fifo_get_validsize(&fifo_tx);
    }
    else
    {
        valid_size = fifo_get_validsize(&fifo_rx);
    }
    return valid_size;
}
/*!
 *  @brief        Discard the UART FIFO's data：Uart_TxBuffer,Uart_RxBuffer
 *  @param        mode：UART_TX->Uart_TxBuffer,UART_RX->Uart_RxBuffer
 *  @return
 *  @note
*/
void BSP_USART_DiscardData(mode_t mode)
{
    if(mode == UART_TX)
    {
        fifo_discard_data(&fifo_tx);
    }
    else
    {
        fifo_discard_data(&fifo_rx);
    }
}
/*!
 *  @brief        Initialize the SPI
 *  @note
*/
void BSP_SPI_Initial(void)
{
    CLK_PeripheralClockConfig(CLK_Peripheral_SPI, ENABLE);
    SPI_DeInit();
    SPI_Init(SPI_FirstBit_MSB, SPI_BaudRatePrescaler_2,
             SPI_Mode_Master, SPI_CPOL_Low, SPI_CPHA_1Edge,
             SPI_Direction_2Lines_FullDuplex, SPI_NSS_Soft);
    SPI_Cmd(ENABLE);
    GPIO_Init(PORT_SPI, PIN_MISO, GPIO_Mode_In_PU_No_IT);
    GPIO_Init(PORT_SPI, PIN_SCLK, GPIO_Mode_Out_PP_High_Slow);
    GPIO_Init(PORT_SPI, PIN_MOSI, GPIO_Mode_Out_PP_High_Slow);
}
/*!
 *  @brief        Exchange a byte via the SPI bus
 *  @note
*/
INT8U BSP_SPI_ExchangeByte(INT8U w_data)
{
    while(RESET == SPI_GetFlagStatus(SPI_FLAG_TXE));
    SPI_SendData(w_data);
    while(RESET == SPI_GetFlagStatus(SPI_FLAG_RXNE));
    return (SPI_ReceiveData());
}