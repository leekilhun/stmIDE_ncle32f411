/*
 * hw.c
 *
 *  Created on: Nov 6, 2021
 *      Author: gns2l
 */



#include "hw.h"



bool hwInit(void)
{
  bool ret = true;

  ret &= bspInit();

  ret &= rtcInit();
  ret &= resetInit();
  ret &= cliInit();
  ret &= ledInit();
  ret &= gpioInit();
  ret &= uartInit();

  ret &= logInit();
  ret &= buttonInit();
  ret &= flashInit();

  ret &= spiInit();
  ret &= i2cInit();
  ret &= canInit();

  return ret;
}
