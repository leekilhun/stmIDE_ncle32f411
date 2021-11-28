/*
 * main.c
 *
 *  Created on: Nov 6, 2021
 *      Author: gns2l
 */



#include "main.h"





int main(void)
{
  hwInit();
  moduleInit();
  apInit();
  apMain();

  return 0;
}
