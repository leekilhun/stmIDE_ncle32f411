/*
 * ap.c
 *
 *  Created on: Nov 6, 2021
 *      Author: gns2l
 */

#include "ap.h"
#include "tickTimer.h"


uint8_t runTimer;
uint8_t lcdTimer;
uint8_t performTimer;
uint32_t count ;

static void lcdMain(void);


void apInit(void)
{
  runTimer = tickTimer_Start();
  lcdTimer = tickTimer_Start();
  performTimer = tickTimer_Start();

  count = 0;
  //uartOpen(_DEF_UART1, 115200);
  cliOpen(_DEF_UART1, 57600);
  //canOpen(_DEF_CAN1, CAN_NORMAL, CAN_CLASSIC, CAN_500K, CAN_500K);
  ledOff(_DEF_LED1);
}

void apMain(void)
{
  while(1)
  {
    if (tickTimer_MoreThan(runTimer, 2000))
    {
      ledToggle(_DEF_LED1);
      runTimer = tickTimer_Start();
    }
    cliMain();
    lcdMain();
    //count = tickTimer_GetElaspTime(lcdTimer);
    /*if (uartAvailable(_DEF_UART1) > 0 )
    {
      uint8_t rx_data ;
      rx_data = uartRead(_DEF_UART1);
      uartPrintf(_DEF_UART1, "Uart2 Rx %c %x\n",rx_data,rx_data);
    }*/

  }
}



void lcdMain(void)
{

  int16_t x;
  int16_t y;
  static uint32_t pre_time;
  McpMode mode;
  McpBaud baud;

  if (lcdIsInit() != true)
  {
    return;
  }



  if (0/*lcdDrawAvailable() == true*/)
  {
    int elasp = tickTimer_GetElaspTime(lcdTimer);
    lcdTimer = tickTimer_Start();


    lcdClearBuffer(black);
    lcdSetFont(LCD_FONT_HAN);
    lcdPrintf(24,16*0, green, "[I2C 통신]");
    lcdPrintf(24,16*1, green, "[%dms]",elasp);

    if (tickTimer_MoreThan(performTimer, 1000))
    {
      lcdPrintf(24,16*2, red, "[%d cnt]",++count);
      performTimer = tickTimer_Start();
    }
  }



  if (millis()-pre_time >= (1000/30) && lcdDrawAvailable() == true)
  {
    pre_time = millis();

    lcdClearBuffer(black);

    lcdSetFont(LCD_FONT_HAN);
    lcdPrintf(24,16*0, green, "[CAN 통신]");

    lcdSetFont(LCD_FONT_07x10);

    mode = mcp2515GetMode(_MODULE_MCP2515_CH1);
    baud = mcp2515GetBaud(_MODULE_MCP2515_CH1);

    x = 0;
    y = 18 + 12*0;
    switch(mode)
    {
      case MCP_MODE_NORMAL:
        lcdPrintf(x, y, white, "Mode : Normal");
        break;
      case MCP_MODE_SLEEP:
        lcdPrintf(x, y, white, "Mode : Sleep");
        break;
      case MCP_MODE_LOOPBACK:
        lcdPrintf(x, y, white, "Mode : Loopback");
        break;
      case MCP_MODE_LISTEN:
        lcdPrintf(x, y, white, "Mode : Listen");
        break;
      case MCP_MODE_CONFIG:
        lcdPrintf(x, y, white, "Mode : Config");
        break;
    }

    x = 0;
    y = 18 + 12*1;
    switch(baud)
    {
      case MCP_BAUD_100K:
        lcdPrintf(x, y, white, "Baud : 100Kbps");
        break;
      case MCP_BAUD_125K:
        lcdPrintf(x, y, white, "Baud : 125Kbps");
        break;
      case MCP_BAUD_250K:
        lcdPrintf(x, y, white, "Baud : 250Kbps");
        break;
      case MCP_BAUD_500K:
        lcdPrintf(x, y, white, "Baud : 500Kbps");
        break;
      case MCP_BAUD_1000K:
        lcdPrintf(x, y, white, "Baud : 1Mbps");
        break;
    }

    lcdRequestDraw();
  }

}

