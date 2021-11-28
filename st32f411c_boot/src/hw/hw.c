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

  //if (resetGetCount() == 2)
  //{
  // resetToSysBoot();
  //}

  //logPrintf("[ Firmware Begin... ]\r\n");
  //logPrintf("Booting..Name \t\t: %s\r\n", _DEF_BOARD_NAME);
  //logPrintf("Booting..Ver  \t\t: %s\r\n", _DEF_FIRMWATRE_VERSION);


  //ret &= usbInit();
  //ret &= usbBegin(USB_CDC_MODE);
  //ret &= uartOpen(_DEF_UART1, 115200);
  //ret &= canInit();
  //ret &= i2cInit();

  //logBoot(false);

  ret &= buttonInit();
  ret &= ledInit();
  ret &= flashInit();


  return ret;
}
