/*
 * hw.c
 *
 *  Created on: Nov 6, 2021
 *      Author: gns2l
 */



#include "hw.h"


extern uint32_t _flash_tag_addr;
extern uint32_t __isr_vector_addr;


__attribute__((section(".tag"))) firm_tag_t firm_tag =
    {
        .magic_number = 0xAAAA5555,

        //-- fw info
        //
        .addr_tag = (uint32_t)&_flash_tag_addr,
        .addr_fw  = (uint32_t)&__isr_vector_addr,
        .size_tag = 1024,


        //-- tag info
        //
    };


__attribute__((section(".version"))) firm_version_t firm_ver =
    {
        "V211111R2",
        "Firmware"
    };


bool hwInit(void)
{
  bool ret = true;

  ret &= bspInit();

  ret &= rtcInit();
  ret &= resetInit();
  ret &= cliInit();
  ret &= ledInit();

  ret &= uartInit();

  ret &= logInit();
  ret &= buttonInit();
  ret &= flashInit();

  return ret;
}
