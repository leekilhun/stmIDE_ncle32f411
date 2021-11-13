/*
 * ap.c
 *
 *  Created on: Nov 6, 2021
 *      Author: gns2l
 */

#include "ap.h"
#include "tickTimer.h"


uint8_t runTimer;


void cliBoot(cli_args_t *args);


void apInit(void)
{
  runTimer = tickTimer_Start();
  //uartOpen(_DEF_UART1, 115200);
  cliOpen(_DEF_UART1, 57600);
  ledOff(_DEF_LED1);

  cliAdd("boot", cliBoot);

}

void apMain(void)
{
  while(1)
  {
    if (tickTimer_MoreThan(runTimer, 500))
    {
      ledToggle(_DEF_LED1);
      runTimer = tickTimer_Start();
    }
    cliMain();
    /*if (uartAvailable(_DEF_UART1) > 0 )
    {
      uint8_t rx_data ;
      rx_data = uartRead(_DEF_UART1);
      uartPrintf(_DEF_UART1, "Uart2 Rx %c %x\n",rx_data,rx_data);
    }*/

  }
}



void cliBoot(cli_args_t *args)
{
  bool ret = false;


  if (args->argc == 1 && args->isStr(0, "info") == true)
  {
    firm_version_t *p_boot_ver = (firm_version_t *)(FLASH_ADDR_BOOT_VER);


    cliPrintf("boot ver   : %s\n", p_boot_ver->version);
    cliPrintf("boot name  : %s\n", p_boot_ver->name);
    cliPrintf("boot param : 0x%X\n", rtcBackupRegRead(0));

    ret = true;
  }

  if (args->argc == 1 && args->isStr(0, "jump_boot") == true)
  {
    resetToBoot(0);
    ret = true;
  }

  if (args->argc == 1 && args->isStr(0, "jump_fw") == true)
  {
    rtcBackupRegWrite(0, 0);
    ret = true;
  }

  if (ret != true)
  {
    cliPrintf("boot info\n");
  }
}

