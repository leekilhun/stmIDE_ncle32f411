/*
 * hw.c
 *
 *  Created on: Nov 6, 2021
 *      Author: gns2l
 */

#include "hw.h"

__attribute__((section(".version"))) firm_version_t boot_ver =
    {
        "B211111R1",
        "Bootloader"
    };



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


  return ret;
}
