/**@file  	    	RingBuffer.h   
* @brief            环形缓冲区设计
* @details          提供一个无边界的字节流服务
* @author           oyz
* @date             2024.10.22
* @version          1.0
* @copyright        Shenzhen DX-SMART Technology Co.Ltd
**********************************************************************************
*/
#ifndef _RINGBUFFER_H_
#define _RINGBUFFER_H_

#include "MyTypeDef.h"
#include <string.h>
#include <stdbool.h>
/*定义缓冲区大小*/
#define BUFFER_MAX_SIZE   512
typedef void (*lock_func)(void);
typedef struct
{
  INT8U *buf; //缓冲区
  INT16U buf_size; //缓冲区大小
  INT16U valid_size; //有效数据大小
  INT8U *pWrite; //写指针
  INT8U *pRead; //读指针
  void (*lock)(void); //互斥上锁
  void (*unlock)(void); //互斥解锁
}_fifo_t;
/*
================================================================================
-------------------------------Function declaration-----------------------------
================================================================================
*/
/*注册缓冲区*/
void fifo_register(_fifo_t *pfifo,INT8U *pfifo_buf,INT16U size,lock_func lock,lock_func unlock);
/*释放缓冲区*/
void fifo_release(_fifo_t *pfifo);
/*向缓冲区写数据*/
INT16U fifo_write(_fifo_t *pfifo,INT8U *pbuf,INT16U size);
/*从缓冲区读数据*/
INT16U fifo_read(_fifo_t *pfifo,INT8U *pbuf,INT16U size);
/*获取缓冲区空间大小 */
INT16U fifo_get_totalsize(_fifo_t *pfifo);
/*获取缓冲区空闲空间大小*/
INT16U fifo_get_freesize(_fifo_t *pfifo);
/*获取缓冲区有效数据大小*/
INT16U fifo_get_validsize(_fifo_t *pfifo);
/*丢弃缓冲区中数据*/
void fifo_discard_data(_fifo_t *pfifo);
/*互斥上锁*/
void lock(void);
/*互斥解锁*/
void unlock(void);
#endif