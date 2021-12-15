/*
 * adc.c
 *
 *  Created on: Dec 15, 2021
 *      Author: gns2l
 */



#include "adc.h"
#include "cli.h"

#ifdef _USE_HW_ADC

#define ADC_MAX_CH    HW_ADC_MAX_CH

#ifdef _USE_HW_CLI
static void cliAdc(cli_args_t *args);
#endif


typedef struct
{
  bool                    is_init;
  ADC_HandleTypeDef      *hADCx;
  uint32_t                adc_channel;
} adc_tbl_t;


ADC_HandleTypeDef hadc1;
DMA_HandleTypeDef hdma_adc1;

static adc_tbl_t adc_tbl[ADC_MAX_CH];
static uint16_t adc_data[ADC_MAX_CH];

static uint32_t adcRead(uint8_t ch);

/*
static uint32_t adcRead8(uint8_t ch);
static uint32_t adcRead10(uint8_t ch);
static uint32_t adcRead12(uint8_t ch);
static uint32_t adcRead16(uint8_t ch);
 */

static uint32_t adcConvVoltage(uint8_t ch, uint32_t adc_value);
static uint32_t adcConvCurrent(uint8_t ch, uint32_t adc_value);


bool adcInit(void)
{
  bool ret = true ;
  uint32_t ch;

  for (int i=0; i<ADC_MAX_CH; i++)
  {
    adc_tbl[i].is_init = false;
  }

  ADC_ChannelConfTypeDef sConfig = {0};

  /** Common config
   */

  hadc1.Instance = ADC1;
  hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
  hadc1.Init.Resolution = ADC_RESOLUTION_12B;
  hadc1.Init.ScanConvMode = ENABLE;
  hadc1.Init.ContinuousConvMode = DISABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 1;
  hadc1.Init.DMAContinuousRequests = DISABLE;
  hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    ret = false;
  }


  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
   */
  sConfig.Channel = ADC_CHANNEL_TEMPSENSOR;
  sConfig.Rank = 1;
  sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    ret = false;
  }


  if (HAL_ADC_Start_DMA(&hadc1,(uint32_t *)adc_data,ADC_MAX_CH) != HAL_OK)
  {
    ret = false;
  }


  ch = 0;
  adc_tbl[ch].is_init     = true;
  adc_tbl[ch].hADCx       = &hadc1;
  adc_tbl[ch].adc_channel = ADC_CHANNEL_TEMPSENSOR;


#ifdef _USE_HW_CLI
  cliAdd("adc", cliAdc);
#endif

  return ret;
}


bool adcIsInit(uint8_t ch)
{
  return adc_tbl[ch].is_init;
}



uint32_t adcRead(uint8_t ch)
{
  uint32_t adc_value;

  if (adc_tbl[ch].is_init != true)
  {
    return 0;
  }

  adc_value = adc_data[ch];

  return adc_value;
}

/*
uint32_t adcRead8(uint8_t ch)
{
  return adcRead(ch)>>4;
}
uint32_t adcRead10(uint8_t ch)
{
  return adcRead(ch)>>2;
}
uint32_t adcRead12(uint8_t ch)
{
  return adcRead(ch);
}
uint32_t adcRead16(uint8_t ch)
{
  return adcRead(ch)<<4;
}
 */

uint32_t adcReadVoltage(uint8_t ch)
{
  return adcConvVoltage(ch, adcRead(ch));
}

uint32_t adcReadCurrent(uint8_t ch)
{

  return adcConvCurrent(ch, adcRead(ch));
}

uint32_t adcConvVoltage(uint8_t ch, uint32_t adc_value)
{
  uint32_t ret = 0;

  switch(ch)
  {
    case 0:
    case 1:
      ret  = (uint32_t)((adc_value * 3300 * 10) / (4095*10));
      ret += 5;
      ret /= 10;
      break;

    case 2:
      break;

  }

  return ret;
}

uint32_t adcConvCurrent(uint8_t ch, uint32_t adc_value)
{
  return 0;
}



/**
  * @brief This function handles DMA2 stream0 global interrupt.
  */
void DMA2_Stream0_IRQHandler(void)
{
  HAL_DMA_IRQHandler(&hdma_adc1);
}









void HAL_ADC_MspInit(ADC_HandleTypeDef* adcHandle)
{
#if 0
  GPIO_InitTypeDef GPIO_InitStruct = {0};
#endif
  if(adcHandle->Instance==ADC1)
  {

    /* DMA controller clock enable */
    __HAL_RCC_DMA2_CLK_ENABLE();

    /* ADC1 clock enable */
    __HAL_RCC_ADC1_CLK_ENABLE();
#if 0
    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**ADC1 GPIO Configuration
        PA6     ------> ADC1_IN6
     */
    GPIO_InitStruct.Pin = GPIO_PIN_6;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
#endif

    /* ADC1 DMA Init */
    /* ADC1 Init */
    hdma_adc1.Instance = DMA2_Stream0;
    hdma_adc1.Init.Channel = DMA_CHANNEL_0;
    hdma_adc1.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_adc1.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_adc1.Init.MemInc = DMA_MINC_ENABLE;
    hdma_adc1.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
    hdma_adc1.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
    hdma_adc1.Init.Mode = DMA_CIRCULAR;
    hdma_adc1.Init.Priority = DMA_PRIORITY_LOW;
    hdma_adc1.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
    if (HAL_DMA_Init(&hdma_adc1) != HAL_OK)
    {
      Error_Handler();
    }
    __HAL_LINKDMA(adcHandle,DMA_Handle,hdma_adc1);

#if 0
    /* ADC1 interrupt Init */
    HAL_NVIC_SetPriority(ADC_IRQn, 0, 5);
    HAL_NVIC_EnableIRQ(ADC_IRQn);
#endif
    /* DMA2_Stream0_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(DMA2_Stream0_IRQn, 0, 5);
    HAL_NVIC_EnableIRQ(DMA2_Stream0_IRQn);
  }
}




void HAL_ADC_MspDeInit(ADC_HandleTypeDef* adcHandle)
{

  if(adcHandle->Instance==ADC1)
  {
    /* Peripheral clock disable */
    __HAL_RCC_ADC1_CLK_DISABLE();
#if 0
    /**ADC1 GPIO Configuration
    PB0     ------> ADC1_IN8
     */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_6);
#endif
    /* ADC1 DMA DeInit */
    HAL_DMA_DeInit(adcHandle->DMA_Handle);
#if 0
    /* ADC1 interrupt Deinit */
    HAL_NVIC_DisableIRQ(ADC_IRQn);
#endif
  }
}





#ifdef _USE_HW_CLI
void cliAdc(cli_args_t *args)
{
  bool ret = true;
  uint32_t line ;

  if (args->argc == 1)
  {
    if (args->isStr(0, "show") == true)
    {
      line = 0;
      while(cliKeepLoop())
      {
        for (int i=0; i<ADC_MAX_CH; i++)
        {
          cliPrintf("[%d] %04d ",line, adcRead(i));
        }
        cliPrintf("\r\n");
        if (line >3)
        {
          cliPrintf("\x1B[%dA", 5);
          line = 0;
        }
        else
        {
          line++;
        }
        delay(50);
      }
    }
    else if (args->isStr(0, "info") == true)
    {
      for (int i=0; i<ADC_MAX_CH; i++)
      {
        cliPrintf("adc channel[%d] init      : %d\n", i, adcIsInit(i));
      }
    }
    else
    {
      ret = false;
    }
  }
  else if (args->argc == 2)
  {
    if (args->isStr(0, "show") == true && args->isStr(1, "voltage") == true)
    {
      line = 0;
      while(cliKeepLoop())
      {
        for (int i=0; i<ADC_MAX_CH; i++)
        {
          uint32_t adc_data;

          adc_data = adcReadVoltage(i);

          cliPrintf("[%d] %d.%02dV ", line, adc_data/100, adc_data%100);
        }
        cliPrintf("\r\n");
        if (line >3)
        {
          cliPrintf("\x1B[%dA", 5);
          line = 0;
        }
        else
        {
          line++;
        }
        delay(50);
      }
    }
    else
    {
      ret = false;
    }
  }
  else
  {
    ret = false;
  }


  if (ret == false)
  {
    cliPrintf( "adc info\n");
    cliPrintf( "adc show\n");
    cliPrintf( "adc show voltage\n");
  }
}
#endif







#endif /*_USE_HW_ADC*/
