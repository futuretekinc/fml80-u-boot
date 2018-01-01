/*
 * mux.c
 *
 * (C) Copyright 2016
 * Heiko Schocher, DENX Software Engineering, hs@denx.de.
 *
 * Based on:
 * Copyright (C) 2011 Texas Instruments Incorporated - http://www.ti.com/
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <asm/arch/sys_proto.h>
#include <asm/arch/hardware.h>
#include <asm/arch/mux.h>
#include <asm/io.h>
#include <i2c.h>
#include "board.h"

static struct module_pin_mux uart0_pin_mux[] = {
	{OFFSET(uart0_rxd), (MODE(0) | PULLUDEN | RXACTIVE)},	/* UART0_RXD */
	{OFFSET(uart0_txd), (MODE(0) | PULLUDDIS)},		/* UART0_TXD */
	{-1},
};

static struct module_pin_mux uart1_pin_mux[] = {
	{OFFSET(uart1_rxd), (MODE(0) | PULLUDDIS | RXACTIVE)},	/* UART1_RXD */
	{OFFSET(uart1_txd), (MODE(0) | PULLUDDIS)},		/* UART1_TXD */
	{-1},
};

static struct module_pin_mux uart2_pin_mux[] = {
	{OFFSET(spi0_sclk), (MODE(1) | PULLUDDIS | RXACTIVE)},	/* UART2_RXD */
	{OFFSET(spi0_d0), (MODE(1) | PULLUDDIS)},		/* UART2_TXD */
	{-1},
};


static struct module_pin_mux mmc0_pin_mux[] = {
	{OFFSET(mmc0_dat3), (MODE(0) | RXACTIVE | PULLUDDIS)},	/* MMC0_DAT3 */
	{OFFSET(mmc0_dat2), (MODE(0) | RXACTIVE | PULLUDDIS)},	/* MMC0_DAT2 */
	{OFFSET(mmc0_dat1), (MODE(0) | RXACTIVE | PULLUDDIS)},	/* MMC0_DAT1 */
	{OFFSET(mmc0_dat0), (MODE(0) | RXACTIVE | PULLUDDIS)},	/* MMC0_DAT0 */
	{OFFSET(mmc0_clk), (MODE(0) | RXACTIVE | PULLUP_EN)},	/* MMC0_CLK */
	{OFFSET(mmc0_cmd), (MODE(0) | RXACTIVE | PULLUDDIS)},	/* MMC0_CMD */
	{OFFSET(spi0_cs1), (MODE(5) | RXACTIVE | PULLUDDIS)},	/* MMC0_CD */
	{-1},
};

static struct module_pin_mux mmc1_pin_mux[] = {
	{OFFSET(gpmc_ad7), (MODE(1) | RXACTIVE | PULLUP_EN)},	/* MMC1_DAT3 */
	{OFFSET(gpmc_ad6), (MODE(1) | RXACTIVE | PULLUP_EN)},	/* MMC1_DAT3 */
	{OFFSET(gpmc_ad5), (MODE(1) | RXACTIVE | PULLUP_EN)},	/* MMC1_DAT3 */
	{OFFSET(gpmc_ad4), (MODE(1) | RXACTIVE | PULLUP_EN)},	/* MMC1_DAT3 */
	{OFFSET(gpmc_ad3), (MODE(1) | RXACTIVE | PULLUP_EN)},	/* MMC1_DAT3 */
	{OFFSET(gpmc_ad2), (MODE(1) | RXACTIVE | PULLUP_EN)},	/* MMC1_DAT2 */
	{OFFSET(gpmc_ad1), (MODE(1) | RXACTIVE | PULLUP_EN)},	/* MMC1_DAT1 */
	{OFFSET(gpmc_ad0), (MODE(1) | RXACTIVE | PULLUP_EN)},	/* MMC1_DAT0 */
	{OFFSET(gpmc_csn1), (MODE(2) | RXACTIVE | PULLUDDIS)},	/* MMC1_CLK */
	{OFFSET(gpmc_csn2), (MODE(2) | RXACTIVE | PULLUP_EN)},	/* MMC1_CMD */
	{-1},
};

static struct module_pin_mux i2c0_pin_mux[] = {
	{OFFSET(i2c0_sda), (MODE(0) | RXACTIVE | PULLUDDIS)}, /* I2C_DATA */
	{OFFSET(i2c0_scl), (MODE(0) | RXACTIVE | PULLUDDIS)}, /* I2C_SCLK */
	{-1},
};

static struct module_pin_mux jtag_pin_mux[] = {
	{OFFSET(xdma_event_intr0), (MODE(6) | RXACTIVE | PULLUDDIS)},
	{OFFSET(xdma_event_intr1), (MODE(6) | RXACTIVE | PULLUDDIS)},
	{OFFSET(nresetin_out), (MODE(0) | RXACTIVE | PULLUDDIS)},
	{OFFSET(nnmi), (MODE(0) | RXACTIVE | PULLUDDIS)},
	{OFFSET(tms), (MODE(0) | RXACTIVE | PULLUP_EN)},
	{OFFSET(tdi), (MODE(0) | RXACTIVE | PULLUP_EN)},
	{OFFSET(tdo), (MODE(0) | PULLUP_EN)},
	{OFFSET(tck), (MODE(0) | RXACTIVE | PULLUP_EN)},
	{OFFSET(ntrst), (MODE(0) | RXACTIVE)},
	{OFFSET(emu0), (MODE(0) | RXACTIVE | PULLUP_EN)},
	{OFFSET(emu1), (MODE(0) | RXACTIVE | PULLUP_EN)},
	{OFFSET(pmic_power_en), (MODE(0) | PULLUP_EN)},
	{OFFSET(rsvd2), (MODE(0) | PULLUP_EN)},
	{OFFSET(rtc_porz), (MODE(0) | RXACTIVE | PULLUDDIS)},
	{OFFSET(ext_wakeup), (MODE(0) | RXACTIVE)},
	{OFFSET(enz_kaldo_1p8v), (MODE(0) | RXACTIVE | PULLUDDIS)},
	{OFFSET(usb0_drvvbus), (MODE(0) | PULLUDEN)},
	{OFFSET(usb1_drvvbus), (MODE(0) | PULLUDDIS)},
	{-1},
};

static struct module_pin_mux gpio_pin_mux[] = {
	{OFFSET(uart0_ctsn),(MODE(7) | PULLUDDIS)},			/* gpio1[8] */ /* USER_LED2 */
	{OFFSET(ecap0_in_pwm0_out), (MODE(7) | PULLUP_EN)},	/* gpio0[7] */ /* USER_LED3 */
	{OFFSET(spi0_d1), 	(MODE(7) | PULLUDDIS)},	/* gpio0[4] */ /* LTE1_STATE_0 */
	{OFFSET(spi0_cs0), 	(MODE(7) | PULLUDDIS)},	/* gpio0[5] */ /* LTE1_STATE_1 */
	{OFFSET(uart1_rtsn),(MODE(7) | PULLUDDIS)},	/* gpio0[13] */ /* LTE_RESET */
	{OFFSET(uart1_ctsn),(MODE(7) | PULLUDDIS)},	/* gpio0[12] */ /* W_DISABLE_N */
	{-1},
};

static struct module_pin_mux rgmii1_pin_mux[] = {
	{OFFSET(mii1_txen), MODE(2)},				/* RGMII1_TCTL */
	{OFFSET(mii1_rxdv), MODE(2) | RXACTIVE},	/* RGMII1_RCTL */
	{OFFSET(mii1_txd3), MODE(2)},				/* RGMII1_TD3 */
	{OFFSET(mii1_txd2), MODE(2)},				/* RGMII1_TD2 */
	{OFFSET(mii1_txd1), MODE(2)},				/* RGMII1_TD1 */
	{OFFSET(mii1_txd0), MODE(2)},				/* RGMII1_TD0 */
	{OFFSET(mii1_txclk), MODE(2)},				/* RGMII1_TCLK */
	{OFFSET(mii1_rxclk), MODE(2) | RXACTIVE},	/* RGMII1_RCLK */
	{OFFSET(mii1_rxd3), MODE(2) | RXACTIVE},	/* RGMII1_RD3 */
	{OFFSET(mii1_rxd2), MODE(2) | RXACTIVE},	/* RGMII1_RD2 */
	{OFFSET(mii1_rxd1), MODE(2) | RXACTIVE},	/* RGMII1_RD1 */
	{OFFSET(mii1_rxd0), MODE(2) | RXACTIVE},	/* RGMII1_RD0 */
	{-1},
};

static struct module_pin_mux rgmii2_pin_mux[] = {
	{OFFSET(gpmc_a0), MODE(2)},				/* RGMII2_TCTL */
	{OFFSET(gpmc_a1), MODE(2) | RXACTIVE},	/* RGMII2_RCTL */
	{OFFSET(gpmc_a2), MODE(2)},				/* RGMII2_TD3 */
	{OFFSET(gpmc_a3), MODE(2)},				/* RGMII2_TD2 */
	{OFFSET(gpmc_a4), MODE(2)},				/* RGMII2_TD1 */
	{OFFSET(gpmc_a5), MODE(2)},				/* RGMII2_TD0 */
	{OFFSET(gpmc_a6), MODE(2)},				/* RGMII2_TCLK */
	{OFFSET(gpmc_a7), MODE(2) | RXACTIVE},	/* RGMII2_RCLK */
	{OFFSET(gpmc_a8), MODE(2) | RXACTIVE},	/* RGMII2_RD3 */
	{OFFSET(gpmc_a9), MODE(2) | RXACTIVE},	/* RGMII2_RD2 */
	{OFFSET(gpmc_a10), MODE(2) | RXACTIVE},	/* RGMII2_RD1 */
	{OFFSET(gpmc_a11), MODE(2) | RXACTIVE},	/* RGMII2_RD0 */
	{-1},
};

static struct module_pin_mux mdio_pin_mux[] = {
	{OFFSET(mdio_data), MODE(0) | RXACTIVE | PULLUP_EN},/* MDIO_DATA */
	{OFFSET(mdio_clk), MODE(0) | PULLUP_EN},	/* MDIO_CLK */
	{-1},
};


void enable_uart0_pin_mux(void)
{
	configure_module_pin_mux(uart0_pin_mux);
}

void enable_uart1_pin_mux(void)
{
	configure_module_pin_mux(uart1_pin_mux);
}

void enable_uart2_pin_mux(void)
{
	configure_module_pin_mux(uart2_pin_mux);
}

void enable_uart3_pin_mux(void)
{
}

void enable_uart4_pin_mux(void)
{
}

void enable_uart5_pin_mux(void)
{
}

void enable_i2c0_pin_mux(void)
{
	configure_module_pin_mux(i2c0_pin_mux);
}

void enable_fml80_board_pin_mux(void)
{
	/* Do board-specific muxes. */
	if (board_is_fml80()) {
		puts("FML80 Pin Mux Configuration\n");
		configure_module_pin_mux(mmc0_pin_mux);
		configure_module_pin_mux(mmc1_pin_mux);
		configure_module_pin_mux(i2c0_pin_mux);
		configure_module_pin_mux(gpio_pin_mux);
		configure_module_pin_mux(uart1_pin_mux);
		configure_module_pin_mux(uart2_pin_mux);
		configure_module_pin_mux(jtag_pin_mux);
		configure_module_pin_mux(rgmii1_pin_mux);
		configure_module_pin_mux(rgmii2_pin_mux);
		configure_module_pin_mux(mdio_pin_mux);
	} else {
		puts("Unknown board, cannot configure pinmux.");
		hang();
	}
}
