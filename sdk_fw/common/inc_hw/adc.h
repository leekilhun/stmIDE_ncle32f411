/*
 * adc.h
 *
 *  Created on: Dec 15, 2021
 *      Author: gns2l
 */

#ifndef SRC_COMMON_INC_HW_ADC_H_
#define SRC_COMMON_INC_HW_ADC_H_


#include "hw_def.h"

#ifdef _USE_HW_ADC

#ifdef __cplusplus
extern "C" {
#endif

bool adcInit(void);
bool adcIsInit(uint8_t ch);
uint32_t adcReadVoltage(uint8_t ch);
uint32_t adcReadCurrent(uint8_t ch);
#ifdef __cplusplus
}
#endif

#endif /*_USE_HW_ADC*/


#endif /* SRC_COMMON_INC_HW_ADC_H_ */
