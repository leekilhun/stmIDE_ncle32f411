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
  //uartOpen(_DEF_UART1, 115200);
  cliOpen(_DEF_UART1, 57600);
  ledOff(_DEF_LED1);
}

void apMain(void)
{
  while(1)
  {
    if (tickTimer_MoreThan(runTimer, 1000))
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
