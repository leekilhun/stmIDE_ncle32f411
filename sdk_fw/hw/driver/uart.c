/*
 * uart.c
 *
 *  Created on: Jul 10, 2021
 *      Author: gns2l
 */



#include "uart.h"
#include "qbuffer.h"


#ifdef _USE_HW_UART


#define UART_RX_BUF_LENGTH      256


UART_HandleTypeDef huart2;
DMA_HandleTypeDef hdma_usart2_rx;

typedef struct
{
  bool     is_open;
  uint32_t baud;
} uart_tbl_t;

static uint8_t  rx_buf[UART_RX_BUF_LENGTH];

//static uint8_t  rx_data[UART_MAX_CH];  //for interrupt receive data
static qbuffer_t qbuffer[UART_MAX_CH];
static uart_tbl_t uart_tbl[UART_MAX_CH];

bool uartInit(void)
{
  for (int i=0; i<UART_MAX_CH; i++)
  {
    uart_tbl[i].is_open = false;
    uart_tbl[i].baud = 115200;
  }

  return true;
}

bool uartOpen(uint8_t ch, uint32_t baud)
{
  bool ret = false;

  switch(ch)
  {
    case _DEF_UART1:
    {
      huart2.Instance         = USART2;
      huart2.Init.BaudRate    = baud;
      huart2.Init.WordLength  = UART_WORDLENGTH_8B;
      huart2.Init.StopBits    = UART_STOPBITS_1;
      huart2.Init.Parity      = UART_PARITY_NONE;
      huart2.Init.Mode        = UART_MODE_TX_RX;
      huart2.Init.HwFlowCtl   = UART_HWCONTROL_NONE;
      huart2.Init.OverSampling = UART_OVERSAMPLING_16;
      if (HAL_UART_Init(&huart2) != HAL_OK)
      {
        Error_Handler();
      }

      HAL_UART_DeInit(&huart2);

      qbufferCreate(&qbuffer[ch], &rx_buf[0], 256);

      /* DMA controller clock enable */
      __HAL_RCC_DMA1_CLK_ENABLE();
      /* DMA interrupt init */
      /* DMA1_Stream5_IRQn interrupt configuration */
      HAL_NVIC_SetPriority(DMA1_Stream5_IRQn, 5, 0);
      HAL_NVIC_EnableIRQ(DMA1_Stream5_IRQn);

      if (HAL_UART_Init(&huart2) != HAL_OK)
      {
        ret = false;
      }
      else
      {
        ret = true;
        uart_tbl[ch].is_open = true;
#if 0 /*for interrupt receive data*/
        if (HAL_UART_Receive_IT(&huart2, (uint8_t *)&rx_data[_DEF_UART1], 1) != HAL_OK)
        {
          ret = false;
        }
#endif
        if(HAL_UART_Receive_DMA(&huart2, (uint8_t *)&rx_buf[0], 256) != HAL_OK)
        {
          ret = false;
        }
        qbuffer[ch].in  = qbuffer[ch].len - hdma_usart2_rx.Instance->NDTR;
        qbuffer[ch].out = qbuffer[ch].in;

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
      qbuffer[ch].in = (qbuffer[ch].len - hdma_usart2_rx.Instance->NDTR);
      ret = qbufferAvailable(&qbuffer[ch]);
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
      qbufferRead(&qbuffer[ch], &ret, 1);
    }
    break;
  }

  return ret;
}

uint32_t uartWrite(uint8_t ch, uint8_t *p_data, uint32_t length)
{
  uint32_t ret = 0;
  HAL_StatusTypeDef status;
  switch(ch)
  {
    case _DEF_UART1:
    {
      status = HAL_UART_Transmit(&huart2, p_data, length, 100);
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
  HAL_UART_IRQHandler(&huart2);
}

void DMA1_Stream5_IRQHandler(void)
{
  HAL_DMA_IRQHandler(&hdma_usart2_rx);
}


void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
#if 0 /*for interrupt receive data*/
  if(huart->Instance==USART2)
  {
    qbufferWrite(&qbuffer[_DEF_UART1],(uint8_t*)&rx_data[_DEF_UART1],1);
    HAL_UART_Receive_IT(&huart2, (uint8_t*)&rx_data[_DEF_UART1], 1);
  }
#endif
}


void HAL_UART_MspInit(UART_HandleTypeDef* uartHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(uartHandle->Instance==USART2)
  {
    /* USART2 clock enable */
    __HAL_RCC_USART2_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();


    /**USART2 GPIO Configuration
    PA2     ------> USART2_TX
    PA3     ------> USART2_RX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_2|GPIO_PIN_3;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART2;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* USART2 DMA Init */
    /* USART2_RX Init */
    hdma_usart2_rx.Instance = DMA1_Stream5;
    hdma_usart2_rx.Init.Channel = DMA_CHANNEL_4;
    hdma_usart2_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_usart2_rx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_usart2_rx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_usart2_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_usart2_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_usart2_rx.Init.Mode = DMA_CIRCULAR;
    hdma_usart2_rx.Init.Priority = DMA_PRIORITY_LOW;
    hdma_usart2_rx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
    if (HAL_DMA_Init(&hdma_usart2_rx) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_LINKDMA(uartHandle,hdmarx,hdma_usart2_rx);

    /* USART2 interrupt Init */
    HAL_NVIC_SetPriority(USART2_IRQn, 0, 5);
    HAL_NVIC_EnableIRQ(USART2_IRQn);
  }
}

void HAL_UART_MspDeInit(UART_HandleTypeDef* uartHandle)
{

  if(uartHandle->Instance==USART2)
  {
    __HAL_RCC_USART2_CLK_DISABLE();

    /**USART2 GPIO Configuration
    PA2     ------> USART2_TX
    PA3     ------> USART2_RX
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_2|GPIO_PIN_3);

    /* USART2 DMA DeInit */
    HAL_DMA_DeInit(&hdma_usart2_rx);

    /* USART2 interrupt Deinit */
    HAL_NVIC_DisableIRQ(USART2_IRQn);
  }
}





#endif
