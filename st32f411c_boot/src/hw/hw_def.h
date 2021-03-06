/*
 * hw_def.h
 *
 *  Created on: Nov 6, 2021
 *      Author: gns2l
 */

#ifndef SRC_HW_HW_DEF_H_
#define SRC_HW_HW_DEF_H_

#include "bsp.h"



#define _USE_HW_FLASH


#define _USE_HW_RTC
#define _USE_HW_RESET

#define _USE_HW_LED
#define      HW_LED_MAX_CH          1

#define _USE_HW_BUTTON
#define      HW_BUTTON_MAX_CH       1


#define _USE_HW_UART
#define      HW_UART_MAX_CH         1

//#define _USE_HW_YMODEM
#define _USE_HW_CMD
#define      HW_CMD_MAX_DATA_LENGTH (1024)


#define FLASH_ADDR_TAG              0x8010000
#define FLASH_ADDR_FW               0x8010400
#define FLASH_ADDR_FW_VER           0x8010800

#define FLASH_ADDR_START            0x8010000
#define FLASH_ADDR_END              (FLASH_ADDR_START + (512-64)*1024)




//#define _USE_HW_LOG
//#define      HW_LOG_CH              _DEF_UART1
//#define      HW_LOG_BOOT_BUF_MAX    1024
//#define      HW_LOG_LIST_BUF_MAX    2048

#define _USE_HW_CLI
#define      HW_CLI_CMD_LIST_MAX    16
#define      HW_CLI_CMD_NAME_MAX    16
#define      HW_CLI_LINE_HIS_MAX    4
#define      HW_CLI_LINE_BUF_MAX    64











//#define _USE_HW_CAN
#define      HW_CAN_MAX_CH          1
#define      HW_CAN_MSG_RX_BUF_MAX  16

//#define _USE_HW_I2C
#define      HW_I2C_MAX_CH          1




#endif /* SRC_HW_HW_DEF_H_ */
