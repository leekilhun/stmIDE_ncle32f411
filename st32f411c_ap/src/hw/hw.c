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

  ret &= resetInit();
  ret &= cliInit();
  ret &= ledInit();
  ret &= gpioInit();

  ret &= logInit();
  ret &= uartInit();
  uartOpen(_DEF_UART1, 57600);

  logOpen(_DEF_UART1, 57600);
  logPrintf("\r\n[ Firmware Begin... ]\r\n");
  logPrintf("Booting..Name \t\t: %s\r\n", _DEF_BOARD_NAME);
  logPrintf("Booting..Ver  \t\t: %s\r\n", _DEF_FIRMWATRE_VERSION);
  logPrintf("Sys Clk       \t\t: %d Mhz\r\n", HAL_RCC_GetSysClockFreq()/1000000);


  ret &= rtcInit();
  ret &= adcInit();

  logBoot(false);

  ret &= buttonInit();
  ret &= flashInit();

  ret &= spiInit();
  ret &= i2cInit();
  //ret &= canInit();

  return ret;
}
