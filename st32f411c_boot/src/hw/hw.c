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

  ret &= uartInit();
  ret &= cliInit();

  ret &= buttonInit();
  ret &= ledInit();
  ret &= flashInit();
  ret &= ymodemInit();


  return ret;
}
