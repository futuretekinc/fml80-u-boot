/*
 * board.h
 *
 * (C) Copyright 2016
 * Heiko Schocher, DENX Software Engineering, hs@denx.de.
 *
 * Based on:
 * TI AM335x boards information header
 *
 * Copyright (C) 2011, Texas Instruments, Incorporated - http://www.ti.com/
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#ifndef _BOARD_H_
#define _BOARD_H_

/* Definition to control the GPIOs (for LEDs and Reset) */
#define GPIO_TO_PIN(bank, gpio) (32 * (bank) + (gpio))

static inline int board_is_fml80(void)
{
	return 1;
}

/*
 * Definitions for pinmuxing header and Board ID strings
 */
#define BOARD_ID_STR "#### SDBOOT ####\nFutureTek FML80\n"

/*
 * Definitions for GPIO pin assignments
 */
# define LED2_GPIO   		GPIO_TO_PIN(1, 8)
# define LED3_GPIO   		GPIO_TO_PIN(0, 7)
# define LTE_STATE0_GPIO   	GPIO_TO_PIN(0, 4)
# define LTE_STATE1_GPIO  	GPIO_TO_PIN(0, 5)
# define LTE_RESET_GPIO 	GPIO_TO_PIN(0, 13)
# define W_DISABLE_GPIO 	GPIO_TO_PIN(0, 12)


/* Reset is on GPIO pin 29 of GPIO bank 1 */
#define RESET_MASK	(0x1 << 29)

#define HDR_MAGIC	0x43485342
#define HDR_ETH_ALEN	6
#define HDR_NAME_LEN	8
#define HDR_REV_LEN	8
#define HDR_SER_LEN	16
#define HDR_ROOT_LEN	12
#define HDR_FATC_LEN	12

/*
* SHC parameters held in On-Board I²C EEPROM device.
*
* Header Format
*
*  Name     Size   Contents
*-------------------------------------------------------------
*  Magic     4     0x42 0x53 0x48 0x43  [BSHC]
*
*  Version   2     0x0100 for v1.0
*
*  Lenght    2     The length of the complete structure, not only this header
*
*  Eth-MAC   6     Ethernet MAC Address
*                  SHC Pool: 7C:AC:B2:00:10:01 - TBD
*
*  --- Further values follow, not important for Bootloader ---
*/

struct  fml80_eeprom {
	u32  magic;
	u16  version;
	u16  lenght;
	uint8_t mac_addr[HDR_ETH_ALEN];
};

void enable_uart0_pin_mux(void);
void enable_fml80_board_pin_mux(void);
void enable_fml80_board_pwm_pin_mux(void);

#endif
