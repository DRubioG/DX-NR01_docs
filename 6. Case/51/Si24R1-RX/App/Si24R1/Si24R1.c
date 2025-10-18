/**@file  	    si24r1.c
* @brief           si24r1 low level operations and configurations.
* @author           oyz
* @date             2024.10.22
* @version          1.0
* @copyright        Shenzhen DX-SMART Technology Co.Ltd
**********************************************************************************
*/
#include "Si24R1.h"
#include <intrins.h>
#include <string.h>

uchar const TX_ADDRESS[TX_ADDR_WIDTH]={0x01,0x02,0x03,0x04,0x05};
uchar const RX_ADDRESS[RX_ADDR_WIDTH]={0x01,0x02,0x03,0x04,0x05};

idata unsigned char rx_buf[R_RX_PAYLOAD_WIDTH];

idata unsigned char tx_buf[W_RX_PAYLOAD_WIDTH];

//uchar flag;
sbit LED = P1^0;

uchar	bdata sta;
//uchar DATA = 0x01;


//sbit  RX_DR     = sta^6;
//sbit  TX_DS     = sta^5;
//sbit  MAX_RT    = sta^4;

//Si24R1 模块引脚位定义
sbit CE  =  P1^2;
sbit CSN =  P1^3;
sbit SCK =  P1^7;
sbit MOSI= P1^5;
sbit MISO= P1^6;
sbit IRQ = P1^4;


//初始化IO口，软件SPI
void init_io(void)
{
    CE  = 0;        // 待机
    CSN = 1;        // SPI传输结束
    SCK = 0;        // SPI时钟置低
    IRQ = 1;        // 中断
    LED = 0;        // 关闭指示灯
}


uchar SW_SPI_ReadWriteByte(uchar byte)
{
    uchar   bit_ctr;
    // output 8-bits
    for (bit_ctr = 0; bit_ctr < 8; bit_ctr++)
    {
        MOSI = (byte & 0x80);   // output 'byte' MSB to MOSI
        byte = (byte << 1);     // shift next bit into MSB..
        SCK = 1;                // Set SCK high.. 24R1 read 1-bit from MOSI and output 1-bit to MISO 
        byte |= MISO;           // capture current MISO bit 
        SCK = 0;                // ..then set SCK low again
    }

    return (byte);  // return read byte
}

//Si24R1写寄存器
//SPI写寄存器
//reg:指定寄存器地址
//value:写入的值
uchar Si24R1_Write_Reg(uchar reg,uchar value)
{
	uchar status;
	CSN=0;      //使能SPI传输
	status=SW_SPI_ReadWriteByte(reg);   //读取寄存器内容
	SW_SPI_ReadWriteByte(value);  //写入寄存器的值
	CSN=1;     //SPI传输结束

	return(status);       			//返回状态值
}


//Si24R1读寄存器
//SPI写寄存器
//reg:指定寄存器地址
//value:写入的值
uchar Si24R1_Read_Reg(uchar reg)
{
	uchar reg_val;
	CSN=0;      //使能SPI传输
	SW_SPI_ReadWriteByte(reg);  //写入寄存器的值
	reg_val=SW_SPI_ReadWriteByte(NOP);  //空操作，用来读取状态寄存器
	CSN=1;     //SPI传输结束

	return reg_val;       			//返回状态值
}



//Si24R1写数据
//reg:寄存器(位置)
//*pBuf:数据指针
//len:数据长度
//返回值,此次读到的状态寄存器值
uchar Si24R1_Write_Buf(uchar reg, uchar *pBuf, uchar len)
{

	uchar status,ctr;
	CSN=0;						 //使能SPI传输					
	status=SW_SPI_ReadWriteByte(reg);	//发送寄存器值(位置),并读取状态值
	for(ctr=0;ctr<len;ctr++)
	{
		SW_SPI_ReadWriteByte(*pBuf++);	//写入数据	 
	
	
	}
	CSN=1;						//SPI传输结束
	return(status);   					//返回状态值
}


//Si24R1读数据
//reg:寄存器(位置)
//*pBuf:数据指针
//len:数据长度
//返回值,此次读到的状态寄存器值
uchar Si24R1_Read_Buf(uchar reg, uchar *pBuf, uchar len)
{
	uchar status,u8_ctr;
	CSN=0;	
	status=SW_SPI_ReadWriteByte(reg);
	for(u8_ctr=0;u8_ctr<len;u8_ctr++)
	{
		pBuf[u8_ctr]=SW_SPI_ReadWriteByte(0xff);	//读数据	 
	
	
	}
	CSN=1;						//SPI传输结束
	return status;   					//返回状态值

}




//启动Si24R1发送一次数据
//txbuf:待发送数据首地址
//返回值:发送完成状况
uchar Si24R1_TxPacket(uchar *txbuf)
{
	uchar sta;
	CE=0;
	Si24R1_Write_Buf(WR_TX_PLOAD,txbuf,W_RX_PAYLOAD_WIDTH);	//写数据到TX BUF  32个字节
	CE=1;
		
	while(IRQ!=0);                           //数据发送完成中断
	sta=Si24R1_Read_Reg(STATUS);					 //读取状态寄存器的值
	Si24R1_Write_Reg(NRF_WRITE_REG+STATUS,sta);			//清除TX_DS或MAX_RT中断标志
	if(sta&MAX_TX)										//达到最大重发次数
	{
		Si24R1_Write_Reg(FLUSH_TX,0xff);		//清除TX FIFO寄存器 
		return 2; 
	
	}
	
	if(sta&TX_OK)											//发送完成
	{

		return 1;
	}
	
	return 0;												//其他原因发送失败
}


//Si24R1接收一次数据
//rxbuf:Si24R1接收一次数据
//返回值:0，接收完成；其他，错误代码
uchar Si24R1_RxPacket(uchar *rxbuf)
{
	uchar sta;		    							   
	 
	sta=Si24R1_Read_Reg(STATUS);  //读取状态寄存器的值    	 
	Si24R1_Write_Reg(NRF_WRITE_REG+STATUS,sta); //清除TX_DS或MAX_RT中断标志
	if(sta&RX_OK)//接收到数据
	{
		
		Si24R1_Read_Buf(RD_RX_PLOAD,rxbuf,R_RX_PAYLOAD_WIDTH);//读取数据
		
		Si24R1_Write_Reg(FLUSH_RX,0xff);//清除RX FIFO寄存器 
		return 0; 
	}	   
	return 1;//没收到任何数据
}		

//设置Si24R1为TX模式
void Si24R1_TX_Mode(void)
{

	CE=0;
	Si24R1_Write_Reg(NRF_WRITE_REG+CONFIG,0x0e);
  	Si24R1_Write_Reg(FLUSH_RX,0xff);
	Si24R1_Write_Reg(FLUSH_TX,0xff);
	CE=1;

}


//设置Si24R1为RX模式
void Si24R1_RX_Mode(void)
{

	CE=0;
	Si24R1_Write_Reg(NRF_WRITE_REG+CONFIG,0x0f);
  	Si24R1_Write_Reg(FLUSH_RX,0xff);
	Si24R1_Write_Reg(FLUSH_TX,0xff);
	CE=1;

}

//Si24R1检测函数
uchar Si24R1_Check(void)
{
	uchar check_in_data[5]={0X55,0XAA,0X55,0XAA,0X55};
	uchar check_out_data[5]={0x00};
	
	CE=0;
	Si24R1_Write_Buf(NRF_WRITE_REG+TX_ADDR,check_in_data,5);
	Si24R1_Read_Buf(NRF_READ_REG+TX_ADDR,check_out_data,5);
	if( (check_out_data[0]==0x55)&&
		(check_out_data[1]==0xAA)&&
		(check_out_data[2]==0x55)&&
		(check_out_data[3]==0xAA)&&
		(check_out_data[4]==0x55))
		return 0;
	else
		return 1;
}


//Si24R1初始化函数
void Si24R1_Init(void)
{
	while(Si24R1_Check())
	{
		printf("No Si24R1 Error\r\n");
		//delay_ms(50);
	}
	
	
	CE=0;
	Si24R1_Write_Reg(NRF_WRITE_REG+RX_PW_P0,R_RX_PAYLOAD_WIDTH);				//选择通道0的有效数据宽度 	    
	Si24R1_Write_Buf(NRF_WRITE_REG+TX_ADDR,(uchar*)TX_ADDRESS,TX_ADDR_WIDTH);		//写TX节点地址 
	Si24R1_Write_Buf(NRF_WRITE_REG+RX_ADDR_P0,(uchar*)RX_ADDRESS,RX_ADDR_WIDTH);	//写 Rx 节点的地址（主要是为了使能 Auto Ack）
	Si24R1_Write_Reg(NRF_WRITE_REG+EN_AA,0x01);			//使能 AUTO ACK EN_AA  开启自动应答
	Si24R1_Write_Reg(NRF_WRITE_REG+EN_RXADDR,0x01);   //使能通道0的接收地址  
	Si24R1_Write_Reg(NRF_WRITE_REG+SETUP_RETR,0x03);  //250us重发延时，自动重发3次
	Si24R1_Write_Reg(NRF_WRITE_REG+RF_CH,2);           //2402MHZ
	Si24R1_Write_Reg(NRF_WRITE_REG+RF_SETUP,0x0f);    //2Mbps,7dbm
	Si24R1_Write_Reg(NRF_WRITE_REG+CONFIG,0x0f);		 //配置基本工作模式的参数;PWR_UP,EN_CRC,16BIT_CRC,接收模式,开启所有中断
	delay_ms(20);
	CE=1;
	
}


void RF_RX_Init(void)
{
	
	init_io();
	Si24R1_Init();
	Si24R1_RX_Mode();
}
