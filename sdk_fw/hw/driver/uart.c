/*
 * uart.c
 *
 *  Created on: Jul 10, 2021
 *      Author: gns2l
 */



#include "uart.h"
#include "qbuffer.h"


#ifdef _USE_HW_UART


#define UART_RX_BUF_LENGTH      1024


typedef struct
{
  bool     is_open;
  uint32_t baud;

  uint8_t  rx_buf[UART_RX_BUF_LENGTH];
  qbuffer_t qbuffer;
  UART_HandleTypeDef *p_huart;
  DMA_HandleTypeDef  *p_hdma_rx;

} uart_tbl_t;


static uart_tbl_t uart_tbl[UART_MAX_CH];


bool uartInit(void)
{
  for (int i=0; i<UART_MAX_CH; i++)
  {
    uart_tbl[i].is_open = false;
    uart_tbl[i].baud = 57600;
  }

  return true;
}

bool uartOpen(uint8_t ch, uint32_t baud)
{
  bool ret = false;

  UART_HandleTypeDef * pHand = uart_tbl[ch].p_huart;
  qbuffer_t que = uart_tbl[ch].qbuffer;

  switch(ch)
  {
    case _DEF_UART1:
    {
      pHand->Instance         = USART2;
      pHand->Init.BaudRate    = baud;
      pHand->Init.WordLength  = UART_WORDLENGTH_8B;
      pHand->Init.StopBits    = UART_STOPBITS_1;
      pHand->Init.Parity      = UART_PARITY_NONE;
      pHand->Init.Mode        = UART_MODE_TX_RX;
      pHand->Init.HwFlowCtl   = UART_HWCONTROL_NONE;
      pHand->Init.OverSampling = UART_OVERSAMPLING_16;
      if (HAL_UART_Init(pHand) != HAL_OK)
      {
        Error_Handler();
      }

      HAL_UART_DeInit(pHand);

      qbufferCreate(&que, &uart_tbl[ch].rx_buf[0], 256);

      /* DMA controller clock enable */
      __HAL_RCC_DMA1_CLK_ENABLE();
      /* DMA interrupt init */
      /* DMA1_Stream5_IRQn interrupt configuration */
      HAL_NVIC_SetPriority(DMA1_Stream5_IRQn, 5, 0);
      HAL_NVIC_EnableIRQ(DMA1_Stream5_IRQn);



      if (HAL_UART_Init(pHand) != HAL_OK)
      {
        ret = false;
      }
      else
      {
        ret = true;
        uart_tbl[ch].is_open = true;
        if(HAL_UART_Receive_DMA(pHand, (uint8_t *)&uart_tbl[ch].rx_buf[0], 256) != HAL_OK)
        {
          ret = false;
        }
        que.in  = que.len - uart_tbl[ch].p_hdma_rx->Instance->NDTR;
        que.out = que.in;
      }
    }
    break;
  }

  return ret;
}

bool uartClose(uint8_t ch)
{
  return true;
}

uint32_t uartAvailable(uint8_t ch)
{
  uint32_t ret = 0;

  switch(ch)
  {
    case _DEF_UART1:
    {
      qbuffer_t que = uart_tbl[ch].qbuffer;
      que.in = (que.len - uart_tbl[ch].p_hdma_rx->Instance->NDTR);
      ret = qbufferAvailable(&que);
    }
    break;
  }

  return ret;
}

bool uartFlush(uint8_t ch)
{
  uint32_t pre_time;

  pre_time = millis();
  while(uartAvailable(ch))
  {
    if (millis()-pre_time >= 10)
    {
      break;
    }
    uartRead(ch);
  }

  return true;
}

uint8_t uartRead(uint8_t ch)
{
  uint8_t ret = 0;

  switch(ch)
  {
    case _DEF_UART1:
    {
      qbuffer_t que = uart_tbl[ch].qbuffer;
      qbufferRead(&que, &ret, 1);
    }
    break;
  }

  return ret;
}

uint32_t uartWrite(uint8_t ch, uint8_t *p_data, uint32_t length)
{
  uint32_t ret = 0;
  HAL_StatusTypeDef status;
  UART_HandleTypeDef * pHand = uart_tbl[ch].p_huart;
  switch(ch)
  {
    case _DEF_UART1:
    {
      status = HAL_UART_Transmit(pHand, p_data, length, 100);
      if (status == HAL_OK)
        ret = length;
    }
    break;
  }

  return ret;
}

uint32_t uartPrintf(uint8_t ch, const char *fmt, ...)
{
  char buf[256];
  va_list args;
  int len;
  uint32_t ret;

  va_start(args, fmt);
  len = vsnprintf(buf, 256, fmt, args);
  ret = uartWrite(ch, (uint8_t *)buf, len);
  va_end(args);

  return ret;
}

uint32_t uartGetBaud(uint8_t ch)
{
  uint32_t ret = 0;

  switch(ch)
  {
    case _DEF_UART1:
     ret = uart_tbl[ch].baud;
     break;
  }

  return ret;
}


void USART2_IRQHandler(void)
{
  HAL_UART_IRQHandler(uart_tbl[_DEF_UART1].p_huart);
}

void DMA1_Stream5_IRQHandler(void)
{
  HAL_DMA_IRQHandler(uart_tbl[_DEF_UART1].p_hdma_rx);
}


#endif
