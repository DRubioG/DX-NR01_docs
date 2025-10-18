/**@file            RingBuffer.c    
* @brief            环形缓冲区设计
* @details          提供一个无边界的字节流服务
* @author           oyz
* @date             2024.10.22
* @version          1.0
* @copyright        Shenzhen DX-SMART Technology Co.Ltd
**********************************************************************************
*/
#include "RingBuffer.h"
/*!< @brief Spinning Lock Flag*/ 
static bool mutex_Flag = false;
/**
 * @brief  互斥上锁
 * @param  
 * @retval 
 */
void lock(void)
{
  while(mutex_Flag);
  mutex_Flag = true;
}
/**
 * @brief  互斥解锁
 * @param  
 * @retval 
 */
void unlock(void)
{
  mutex_Flag = false;
}
/*!
 *  @brief       注册缓冲区            
 *  @param[in]   pfifo _fifo_t结构体指针
 *               pbuf 缓冲区
 *               size 缓冲区大小
 *  @param[out]  
 *  @return      none
 *  @note       
 *     
*/
void fifo_register(_fifo_t *pfifo,INT8U *pfifo_buf,INT16U size,lock_func lock,lock_func unlock)
{
    pfifo->buf = pfifo_buf;
    pfifo->buf_size = size;
    pfifo->valid_size = 0;
    pfifo->pWrite = pfifo->buf;
    pfifo->pRead = pfifo->buf;
    pfifo->lock = lock;
    pfifo->unlock = unlock;
}
/*!
 *  @brief       释放缓冲区            
 *  @param[in]   pfifo _fifo_t结构体指针
 *  @param[out]  
 *  @return      
 *  @note       
 *     
*/
void fifo_release(_fifo_t *pfifo)
{
    pfifo->buf = NULL;
    pfifo->buf_size = 0;
    pfifo->valid_size = 0;
    pfifo->pWrite = 0;
    pfifo->pRead = 0;
    pfifo->lock = NULL;
    pfifo->unlock = NULL;
}
/*!
 *  @brief       向缓冲区写数据            
 *  @param[in]   pfifo _fifo_t结构体指针
 *               pbuf   待写数据
 *               size   待写数据大小
 *  @param[out]  
 *  @return      实际写入大小
 *  @note       
 *     
*/
INT16U fifo_write(_fifo_t *pfifo,INT8U *pbuf,INT16U size)
{
    INT16U w_size = 0,free_size = 0;
    if(size == 0|pbuf == NULL|pfifo == NULL)
    {
      return 0;
    }
    free_size = fifo_get_freesize(pfifo);
    if(free_size == 0)
    {
      return 0;
    }
    if(free_size < size)
    {
      size = free_size;
    }
    w_size = size;
    if(pfifo->lock != NULL)
    {
      pfifo->lock();
    }
    while(w_size-- >0)
    {
      *pfifo->pWrite++ = *pbuf++;
      if(pfifo->pWrite >= (pfifo->buf + pfifo->buf_size))
      {
        pfifo->pWrite = pfifo->buf;
      }
      pfifo->valid_size++;
    }
    if(pfifo->unlock != NULL)
    {
      pfifo->unlock();
    }
    return size;
}
/*!
 *  @brief       从缓冲区读数据            
 *  @param[in]   pfifo _fifo_t结构体指针
 *               pbuf   待读数据
 *               size   待读数据大小
 *  @param[out]  
 *  @return      实际读取大小
 *  @note       
 *     
*/
INT16U fifo_read(_fifo_t *pfifo,INT8U *pbuf,INT16U size)
{
  INT16U r_size = 0,valid_size = 0;
  if(size == 0|pbuf == NULL|pfifo == NULL)
  {
    return 0;
  }
  valid_size = fifo_get_validsize(pfifo);
  if(valid_size == 0)
  {
    return 0;
  }
  if(size > valid_size)
  {
    size = valid_size;
  }
  r_size = size;
  if(pfifo->lock != NULL)
  {
    pfifo->lock();
  }
  while(r_size-- > 0)
  {
    *pbuf++ = *pfifo->pRead++;
    if(pfifo->pRead >= (pfifo->buf + pfifo->buf_size))
    {
      pfifo->pRead = pfifo->buf;
    }
    pfifo->valid_size--;
  }
  if(pfifo->unlock != NULL)
  {
    pfifo->unlock();
  }
  return size;
}
/*!
 *  @brief       获取缓冲区空间大小          
 *  @param[in]   pfifo _fifo_t结构体指针
 *  @param[out]  
 *  @return      缓冲区空间大小
 *  @note       
 *     
*/
INT16U fifo_get_totalsize(_fifo_t *pfifo)
{
  if(pfifo == NULL)
  {
    return 0;
  }
  return pfifo->buf_size;
}
/*!
 *  @brief       获取缓冲区空闲空间大小          
 *  @param[in]   pfifo _fifo_t结构体指针
 *  @param[out]  
 *  @return      缓冲区空闲空间大小
 *  @note       
 *     
*/
INT16U fifo_get_freesize(_fifo_t *pfifo)
{
  INT16U free_size;
  if(pfifo == NULL)
  {
    return 0;
  }
  free_size = pfifo->buf_size - pfifo->valid_size;
  return free_size;
}
/*!
 *  @brief       获取缓冲区有效数据大小          
 *  @param[in]   pfifo _fifo_t结构体指针
 *  @param[out]  
 *  @return      缓冲区有效数据大小 
 *  @note       
 *     
*/
INT16U fifo_get_validsize(_fifo_t *pfifo)
{
  if(pfifo == NULL)
  {
    return 0;
  }
  return pfifo->valid_size;
}
/*!
 *  @brief       丢弃缓冲区中数据         
 *  @param[in]   pfifo _fifo_t结构体指针
 *  @param[out]  
 *  @return      
 *  @note       
 *     
*/
void fifo_discard_data(_fifo_t *pfifo)
{
    pfifo->valid_size = 0;
    pfifo->pWrite = pfifo->buf;
    pfifo->pRead = pfifo->buf;
}