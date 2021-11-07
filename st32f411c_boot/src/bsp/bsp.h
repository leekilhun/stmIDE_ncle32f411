/*
 * bsp.h
 *
 *  Created on: Nov 6, 2021
 *      Author: gns2l
 */

#ifndef SRC_BSP_BSP_H_
#define SRC_BSP_BSP_H_

#include "def.h"

#include "stm32F4xx_hal.h"

#include "stm32f4xx_ll_rcc.h"
#include "stm32f4xx_ll_bus.h"
#include "stm32f4xx_ll_system.h"
#include "stm32f4xx_ll_exti.h"
#include "stm32f4xx_ll_cortex.h"
#include "stm32f4xx_ll_utils.h"
#include "stm32f4xx_ll_pwr.h"
#include "stm32f4xx_ll_dma.h"
#include "stm32f4xx_ll_gpio.h"


bool bspInit(void);
void bspDeInit(void);
void delay(uint32_t time_ms);
uint32_t millis(void);

void Error_Handler(void);

void logPrintf(const char *fmt, ...);

#endif /* SRC_BSP_BSP_H_ */
