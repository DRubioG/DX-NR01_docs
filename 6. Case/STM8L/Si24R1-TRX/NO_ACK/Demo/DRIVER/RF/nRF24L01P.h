/**@file  	    nRF24L01P.h
* @brief            Si24R1 low level operations and configurations.
* @author           oyz
* @date             2024.10.22
* @version          1.0
* @copyright        Shenzhen DX-SMART Technology Co.Ltd
**********************************************************************************
*/
#ifndef nRF24L01P_H
#define nRF24L01P_H

#include "bsp.h"
#include "MyTypeDef.h"
#include "nRF24L01P_REG.h"

#define IRQ_ALL ((1 << RX_DR) | (1 << TX_DS) | (1 << MAX_RT))
/*Data Rate selection*/
typedef enum {DRATE_250K,DRATE_1M,DRATE_2M}L01_DRATE;
/*Power selection*/
typedef enum {POWER_N_0,POWER_N_6,POWER_N_12,POWER_N_18}L01_PWR;
/*Mode selection*/
typedef enum {TX_MODE,RX_MODE}L01_MODE;
/*CE pin level selection*/
typedef enum {CE_LOW,CE_HIGH}CE_STAUS;
/*Si24R1 Power selection*/
typedef enum {  
    POWER_SI24R1_7DBM,  
    POWER_SI24R1_4DBM,  
    POWER_SI24R1_3DBM,  
    POWER_SI24R1_1DBM,  
    POWER_SI24R1_0DBM,  
    POWER_SI24R1_MINUS4DBM,  
    POWER_SI24R1_MINUS6DBM,  
    POWER_SI24R1_MINUS12DBM  
} Si24R1_PWR;  
/*
================================================================================
============================Configurations and Options==========================
================================================================================
*/
#define DYNAMIC_PACKET      1 //1:DYNAMIC packet length, 0:fixed
#define FIXED_PACKET_LEN    32//Packet size in fixed size mode
#define INIT_ADDR           0x01,0x02,0x03,0x04,0x05
/*
================================================================================
==========================List of externally provided functions ================
================================================================================
*/
#define L01_CSN_LOW()      GPIO_ResetBits(PORT_L01_CSN, PIN_L01_CSN)//Pull down the SPI chip select
#define L01_CSN_HIGH()     GPIO_SetBits(PORT_L01_CSN, PIN_L01_CSN)//Pull up the SPI chip select
#define L01_CE_LOW()       GPIO_ResetBits(PORT_L01_CE, PIN_L01_CE)//Set CE low level
#define L01_CE_HIGH()      GPIO_SetBits(PORT_L01_CE, PIN_L01_CE)//Set CE high level
#define GET_L01_IRQ()      GPIO_ReadInputDataBit(PORT_L01_IRQ, PIN_L01_IRQ)//Get the IRQ pin status
#define GET_L01_KEY()      GPIO_ReadInputDataBit(KEY1_PORT, KEY1_PINS)//Get the KEY pin status
#define SPI_ExchangeByte(data)      BSP_SPI_ExchangeByte(data) //Exchange data by the SPI
/*
================================================================================
-------------------------------------Exported APIs------------------------------
================================================================================
*/
/*Set the level status of the CE pin low or high*/
void L01_SetCE(CE_STAUS status);
/*Read the value from the specified register */
INT8U L01_ReadSingleReg(INT8U addr);
/*Read the values of the specified registers and store them in buffer*/
void L01_ReadMultiReg(INT8U start_addr,INT8U *buffer,INT8U size);
/*Write a value to the specified register*/
void L01_WriteSingleReg(INT8U addr,INT8U value);
/*Write buffer to the specified registers */
void L01_WriteMultiReg(INT8U start_addr,INT8U *buffer,INT8U size);
/*Set the nRF24L01 into PowerDown mode */
void L01_SetPowerDown(void);
/*Set the nRF24L01 into PowerUp mode*/
void L01_SetPowerUp(void);
/*Flush the TX buffer*/
void L01_FlushTX(void);
/*Flush the RX buffer*/
void L01_FlushRX(void);
/*Reuse the last transmitted payload*/
void L01_ReuseTXPayload(void);
/*Read the status register of the nRF24L01*/
INT8U L01_ReadStatusReg(void);
/*Clear the IRQ caused by the nRF24L01+*/
void L01_ClearIRQ(INT8U irqMask);
/*Read the IRQ status of the nRF24L01+*/
INT8U L01_ReadIRQSource(void);
/*Read the payload width of the top buffer of the FIFO */
INT8U L01_ReadTopFIFOWidth(void);
/*Read the RX payload from the FIFO and store them in buffer*/
INT8U L01_ReadRXPayload(INT8U *buffer);
/*Write TX Payload to a data pipe,and PRX will return ACK back*/
void L01_WriteTXPayload_Ack(INT8U *buffer,INT8U size);
/*Write TX Payload to a data pipe,and PRX won't return ACK back*/
void L01_WriteTXPayload_NoAck(INT8U *buffer,INT8U size);
/*Write TX Payload to a data pipe when RX mode*/
void L01_WriteRXPayload_InAck(INT8U *buffer,INT8U size);
/*Write Transmit address into TX_ADDR register */
void L01_SetTXAddr(INT8U *Addrbuffer,INT8U Addr_size);
/*Write address for the RX pipe*/
void L01_SetRXAddr(INT8U pipeNum,INT8U *addrBuffer,INT8U addr_size);
/*Set the data rate of the nRF24L01+ */
void L01_SetDataRate(L01_DRATE drate);
/*Set the power of the nRF24L01+ */
void L01_SetPower(L01_PWR power);
void Si24R1_SetPower(Si24R1_PWR power);
/*Set the frequency of the nRF24L01+*/
void L01_WriteHoppingPoint(INT8U freq);
/*Set the nRF24L01+ as TX/RX mode*/
void L01_SetTRMode(L01_MODE mode);
/*Initialize the nRF24L01+ */
void L01_Init(void);
#endif