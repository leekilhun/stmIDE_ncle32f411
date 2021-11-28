/*
 * ap.c
 *
 *  Created on: Nov 6, 2021
 *      Author: gns2l
 */

#include "ap.h"
#include "tickTimer.h"
#include "boot/boot.h"

cmd_t cmd;


void apInit(void)
{
  bool run_boot = false;
  uint8_t reg;

  reg = rtcBackupRegRead(0);

  if (reg & (1<<0))
  {
    run_boot = true;
    reg &= ~(1<<0);
    rtcBackupRegWrite(0, reg);
  }


  if (resetGetCount() == 2)
  {
    run_boot = true;
  }


  if (run_boot == false)
  {
    if (buttonGetPressed(_DEF_BUTTON1) == false)
    {
      if (bootVerifyFw() == true && bootVerifyCrc() == true)
      {
        bootJumpToFw();
      }
    }
  }

  cliOpen(_DEF_UART1, 57600);

  cmdInit(&cmd);
  cmdOpen(&cmd, _DEF_UART1, 57600);


}

void apMain(void)
{
  uint32_t pre_time;
  //uint8_t rx_data;


  pre_time = millis();
  while(1)
  {
    if (millis()-pre_time >= 100)
    {
      pre_time = millis();
      ledToggle(_DEF_LED1);
    }
    /*if (uartAvailable(_DEF_UART1) > 0)
    {
      rx_data = uartRead(_DEF_UART1);
      uartPrintf(_DEF_UART1, "Uart2 Rx %c %x\n",rx_data,rx_data);
    }*/
    //cliMain();

    if (cmdReceivePacket(&cmd) == true)
    {
      bootProcessCmd(&cmd);
    }
  }
}

