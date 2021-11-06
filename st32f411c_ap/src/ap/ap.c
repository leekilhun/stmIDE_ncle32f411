/*
 * ap.c
 *
 *  Created on: Nov 6, 2021
 *      Author: gns2l
 */

#include "ap.h"
#include "tickTimer.h"


uint8_t runTimer;




void apInit(void)
{
  runTimer = tickTimer_Start();
  cliOpen(_DEF_UART1, 57600);
  ledOff(_DEF_LED1);
}

void apMain(void)
{
  while(1)
  {
    if (tickTimer_MoreThan(runTimer, 100))
    {
      ledToggle(_DEF_LED1);
      runTimer = tickTimer_Start();
    }
    cliMain();

  }
}
