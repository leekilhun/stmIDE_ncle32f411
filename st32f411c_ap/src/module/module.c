/*
 * module.c
 *
 *  Created on: Nov 6, 2021
 *      Author: gns2l
 */

#ifndef SRC_MODULE_MODULE_C_
#define SRC_MODULE_MODULE_C_



#include "module.h"


void moduleInit(void)
{

  if (mcp2515Init() != true)
  {
    Error_Handler();
  }

  if (mcp23017Init() != true)
  {
    Error_Handler();
  }

  if (lcdInit() != true)
  {
    Error_Handler();
  }


}

#endif /* SRC_MODULE_MODULE_C_ */
