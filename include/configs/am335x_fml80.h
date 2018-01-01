/*
 * (C) Copyright 2016
 * Heiko Schocher, DENX Software Engineering, hs@denx.de.
 *
 * Based on:
 * am335x_evm.h
 *
 * Copyright (C) 2011 Texas Instruments Incorporated - http://www.ti.com/
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#ifndef __CONFIG_AM335X_FML80_H
#define __CONFIG_AM335X_FML80_H

/*#define	DEBUG	1 */

#include <configs/ti_am335x_common.h>

/* settings we don;t want on this board */
#undef CONFIG_SPL_AM33XX_ENABLE_RTC32K_OSC
#undef CONFIG_CMD_EXT4
#undef CONFIG_CMD_EXT4_WRITE
#undef CONFIG_CMD_SPI

#define CONFIG_CMD_CACHE

#ifndef CONFIG_SPL_BUILD
# define CONFIG_TIMESTAMP
#endif

#define CONFIG_SYS_BOOTM_LEN		(16 << 20)

/* Clock Defines */
#define V_OSCK				24000000  /* Clock output from T2 */
#define V_SCLK				(V_OSCK)

/*
 * in case of SD Card or Network boot we want to have a possibility to
 * debrick the shc, therefore do not read environment from eMMC
 */
#if defined(CONFIG__SDBOOT) || defined(CONFIG_FML80_NETBOOT)
#define CONFIG_SYS_MMC_ENV_DEV		0
#else
#define CONFIG_SYS_MMC_ENV_DEV		1
#endif

/*
 * Info when using boot partitions: As environment resides within first
 * 128 kB, MLO must start at 128 kB == 0x20000
 * ENV at MMC Boot0 Partition - 0/Undefined=user, 1=boot0, 2=boot1,
 * 4..7=general0..3
 */
#define CONFIG_ENV_SIZE				0x1000 /* 4 KB */
#define CONFIG_ENV_OFFSET			0x7000 /* 28 kB */

#define CONFIG_HSMMC2_8BIT

#define CONFIG_ENV_OFFSET_REDUND    0x9000 /* 36 kB */
#define CONFIG_ENV_SIZE_REDUND      CONFIG_ENV_SIZE

#ifndef CONFIG__ICT
/*
 * In builds other than ICT, reset to retry after timeout
 * Define a timeout after which a stopped bootloader continues autoboot
 * (only works with CONFIG_RESET_TO_RETRY)
 */
# define CONFIG_BOOT_RETRY_TIME 30
# define CONFIG_RESET_TO_RETRY
#endif

#define CONFIG_ENV_VARS_UBOOT_CONFIG
#define CONFIG_ENV_VARS_UBOOT_RUNTIME_CONFIG

#ifndef CONFIG_SPL_BUILD
#define CONFIG_EXTRA_ENV_SETTINGS \
	"loadaddr=0x80200000\0" \
	"kloadaddr=0x84000000\0" \
	"fdtaddr=0x85000000\0" \
	"fdt_high=0xffffffff\0" \
	"rdaddr=0x81000000\0" \
	"bootfile=uImage\0" \
	"fdtfile=am335x-shc.dtb\0" \
	"verify=no\0" \
	"ipaddr=192.168.0.111\0"\
	"netmask=255.255.255.0\0"\
	"serverip=192.168.0.115\0" \
	"rootpath=/srv/nfs/shc-rootfs\0" \
	"console=ttyO0,115200n8\0" \
	"optargs=quiet\0" \
	"mmcdev=1\0" \
	"harakiri=0\0" \
	"mmcpart=2\0" \
	"active_root=root1\0" \
	"inactive_root=root2\0" \
	"mmcrootfstype=ext4 rootwait\0" \
	"nfsopts=nolock\0" \
	"static_ip=${ipaddr}:${serverip}:${gatewayip}:${netmask}:${hostname}" \
		"::off\0" \
	"ip_method=none\0" \
	"bootargs_defaults=setenv bootargs " \
		"console=${console} " \
		"${optargs}\0" \
	"mmcargs=run bootargs_defaults;" \
		"setenv bootargs ${bootargs} " \
		"root=${mmcroot} " \
		"rootfstype=${mmcrootfstype} ip=${ip_method}\0" \
	"netargs=setenv bootargs console=${console} " \
		"${optargs} " \
		"root=/dev/nfs " \
		"nfsroot=${serverip}:${rootpath},${nfsopts} rw " \
		"ip=dhcp\0" \
	"bootenv=uEnv.txt\0" \
	"loadbootenv=if fatload mmc ${mmcdev} ${loadaddr} ${bootenv}; then " \
			"echo Loaded environment from ${bootenv}; " \
			"run importbootenv; " \
		"fi;\0" \
	"importbootenv=echo Importing environment variables from uEnv.txt ...; " \
		"env import -t $loadaddr $filesize\0" \
	"loaduimagefat=fatload mmc ${mmcdev} ${kloadaddr} ${bootfile}\0" \
	"loaduimage=ext2load mmc ${mmcdev}:${mmcpart} ${kloadaddr} /boot/${bootfile}\0" \
	"loadfdt=ext2load mmc ${mmcdev}:${mmcpart} ${fdtaddr} /boot/${fdtfile}\0" \
	"netloaduimage=tftp ${loadaddr} ${bootfile}\0" \
	"netloadfdt=tftp ${fdtaddr} ${fdtfile}\0" \
	"mmcboot=echo Booting Linux from ${mmcdevice} ...; " \
		"run mmcargs; " \
		"if run loadfdt; then " \
			"echo device tree detected; " \
			"bootm ${kloadaddr} - ${fdtaddr}; " \
		"else " \
			"bootm ${kloadaddr}; " \
		"fi; \0" \
	"netboot=echo Booting from network ...; " \
		"setenv autoload no; " \
		"dhcp; " \
		"run netloaduimage; " \
		"run netargs; " \
		"echo NFS path: ${serverip}:${rootpath};" \
		"if run netloadfdt; then " \
			"echo device tree detected; " \
			"bootm ${loadaddr} - ${fdtaddr}; " \
		"else " \
			"bootm ${loadaddr}; " \
		"fi; \0" \
	"emmc_erase=if test ${harakiri} = 1 ; then echo erase emmc ...; setenv mmcdev 1; mmc erase 0 200; reset; fi; \0" \
	"mmcpart_gp=mmcpart gp 1 40; \0" \
	"mmcpart_enhance=mmcpart enhance 0 64; \0" \
	"mmcpart_rel_write=mmcpart rel_write 1f; \0" \
	"mmcpart_commit=mmcpart commit 1; \0" \
	"mmc_hw_part=run mmcpart_gp; run mmcpart_enhance; run mmcpart_rel_write; run mmcpart_commit; \0" \
	"led_success=gpio set 22; \0" \
	"fusecmd=mmc dev 1; if mmcpart iscommitted; then echo HW Partitioning already committed; mmcpart list; else run mmc_hw_part; fi; run led_success; \0" \
	"uenv_exec=if test -n $uenvcmd; then " \
			"echo Running uenvcmd ...; " \
			"run uenvcmd; " \
		"fi;\0" \
	"sd_setup=echo SD/MMC-Card detected on device 0; " \
		"setenv mmcdevice SD; " \
		"setenv mmcdev 0; " \
		"setenv mmcpart 2; " \
		"setenv mmcroot /dev/mmcblk${mmcdev}p${mmcpart};\0" \
	"emmc_setup=echo eMMC detected on device 1; " \
		"setenv mmcdevice eMMC; " \
		"setenv mmcdev 1; " \
		"run emmc_erase; " \
		"if test ${active_root} = root2; then " \
			"echo Active root is partition 6 (root2); " \
			"setenv mmcpart 6; " \
		"else " \
			"echo Active root is partition 5 (root1); " \
			"setenv mmcpart 5; " \
		"fi; " \
		"setenv mmcroot /dev/mmcblk${mmcdev}p${mmcpart};\0"
#else
#define CONFIG_EXTRA_ENV_SETTINGS \
	"sarch=arm\0"\
	"baudrate=115200\0"\
	"board=fml80\0"\
	"bootcmd=gpio set 53; i2c mw 0x24 1 0x3e; run findfdt; mmc dev 0; if mmc rescan ; then echo micro SD card found;setenv mmcdev 0;else echo No micro SD card found, setting mmcdev to 1;setenv mmcdev 1;fi;setenv bootpart ${mmcdev}:2;mmc dev ${mmcdev}; if mmc rescan; then gpio set 54; echo SD/MMC found on device ${mmcdev};if run loadbootenv; then echo Loaded environment from ${bootenv};run importbootenv;fi;if test -n $uenvcmd; then echo Running uenvcmd ...;run uenvcmd;fi;gpio set 55; if run loaduimage; then gpio set 56; run loadfdt;run mmcboot;fi;fi;\0"\
	"bootdelay=1\0"\
	"bootdir=/boot\0"\
	"bootenv=uEnv.txt\0"\
	"bootfile=uImage\0"\
	"bootpart=0:2\0"\
	"console=ttyO0,115200n8\0"\
	"cpu=armv7\0"\
	"dfu_alt_info_emmc=rawemmc mmc 0 3751936\0"\
	"dfu_alt_info_mmc=boot part 0 1;rootfs part 0 2;MLO fat 0 1;MLO.raw mmc 100 100;u-boot.img.raw mmc 300 3C0;u-boot.img fat 0 1;uEnv.txt fat 0 1\0"\
	"dfu_alt_info_nand=SPL part 0 1;SPL.backup1 part 0 2;SPL.backup2 part 0 3;SPL.backup3 part 0 4;u-boot part 0 5;kernel part 0 7;rootfs part 0 8\0"\
	"ethact=cpsw\0"\
	"ethaddr=98:84:e3:a7:7c:14\0"\
	"fdt_high=0xffffffff\0"\
	"fdtaddr=0x80F80000\0"\
	"fdtfile=am335x-boneblack.dtb\0"\
	"findfdt=if test $board_name = A33515BB; then setenv fdtfile am335x-evm.dtb; fi; if test $board_name = A335X_SK; then setenv fdtfile am335x-evmsk.dtb; fi;if test $board_name = A335BONE; then setenv fdtfile am335x-bone.dtb; fi; if test $board_name = A335BNLT; then setenv fdtfile am335x-boneblack.dtb; fi\0"\
	"importbootenv=echo Importing environment from mmc ...; env import -t $loadaddr $filesize\0"\
	"kloadaddr=0x80007fc0\0"\
	"loadaddr=0x80200000\0"\
	"loadbootenv=load mmc ${mmcdev} ${loadaddr} ${bootenv}\0"\
	"loadfdt=load mmc ${bootpart} ${fdtaddr} ${bootdir}/${fdtfile}\0"\
	"loadramdisk=load mmc ${mmcdev} ${rdaddr} ramdisk.gz\0"\
	"loaduimage=load mmc ${bootpart} ${kloadaddr} ${bootdir}/${bootfile}\0"\
	"mmcargs=setenv bootargs console=${console} ${optargs} root=${mmcroot} rootfstype=${mmcrootfstype}\0"\
	"mmcboot=echo Booting from mmc ...; run mmcargs; bootm ${kloadaddr} - ${fdtaddr}\0"\
	"mmcdev=0\0"\
	"mmcroot=/dev/mmcblk0p2 ro\0"\
	"mmcrootfstype=ext4 rootwait\0"\
	"mtdids=nand0=omap2-nand.0\0"\
	"mtdparts=mtdparts=omap2-nand.0:128k(SPL),128k(SPL.backup1),128k(SPL.backup2),128k(SPL.backup3),1920k(u-boot),128k(u-boot-env),5m(kernel),-(rootfs)\0"\
	"nandargs=setenv bootargs console=${console} ${optargs} root=${nandroot} rootfstype=${nandrootfstype}\0"\
	"nandboot=echo Booting from nand ...; run nandargs; nand read ${loadaddr} ${nandsrcaddr} ${nandimgsize}; bootm ${loadaddr}\0"\
	"nandimgsize=0x500000\0"\
	"nandroot=ubi0:rootfs rw ubi.mtd=7,2048\0"\
	"nandrootfstype=ubifs rootwait=1\0"\
	"nandsrcaddr=0x280000\0"\
	"netargs=setenv bootargs console=${console} ${optargs} root=/dev/nfs nfsroot=${serverip}:${rootpath},${nfsopts} rw ip=dhcp\0"\
	"netboot=echo Booting from network ...; setenv autoload no; dhcp; tftp ${loadaddr} ${bootfile}; tftp ${fdtaddr} ${fdtfile}; run netargs; bootm ${loadaddr} - ${fdtaddr}\0"\
	"nfsopts=nolock\0"\
	"ramargs=setenv bootargs console=${console} ${optargs} root=${ramroot} rootfstype=${ramrootfstype}\0"\
	"ramboot=echo Booting from ramdisk ...; run ramargs; bootm ${loadaddr} ${rdaddr} ${fdtaddr}\0"\
	"ramroot=/dev/ram0 rw ramdisk_size=65536 initrd=${rdaddr},64M\0"\
	"ramrootfstype=ext2\0"\
	"rdaddr=0x81000000\0"\
	"rootpath=/export/rootfs\0"\
	"soc=am33xx\0"\
	"spiargs=setenv bootargs console=${console} ${optargs} root=${spiroot} rootfstype=${spirootfstype}\0"\
	"spiboot=echo Booting from spi ...; run spiargs; sf probe ${spibusno}:0; sf read ${loadaddr} ${spisrcaddr} ${spiimgsize}; bootm ${loadaddr}\0"\
	"spibusno=0\0"\
	"spiimgsize=0x362000\0"\
	"spiroot=/dev/mtdblock4 rw\0"\
	"spirootfstype=jffs2\0"\
	"spisrcaddr=0xe0000\0"\
	"static_ip=${ipaddr}:${serverip}:${gatewayip}:${netmask}:${hostname}::off\0"\
	"stderr=serial\0"\
	"stdin=serial\0"\
	"stdout=serial\0"\
	"usbnet_devaddr=98:84:e3:a7:7c:14\0"\
	"vendor=futuretek\0"
#endif /* #ifndef CONFIG_SPL_BUILD */

#if defined CONFIG__NETBOOT
/* Network Boot */
# define CONFIG_BOOTCOMMAND \
	"run fusecmd; " \
	"if run netboot; then " \
		"echo Booting from network; " \
	"else " \
		"echo ERROR: Cannot boot from network!; " \
		"panic; " \
	"fi; "

#elif defined CONFIG_FML80_SDBOOT /* !defined CONFIG_FML80_NETBOOT */
/* SD-Card Boot */
# define CONFIG_BOOTCOMMAND \
	"if mmc dev 0; mmc rescan; then " \
		"run sd_setup; " \
	"else " \
		"echo ERROR: SD/MMC-Card not detected!; " \
		"panic; " \
	"fi; " \
	"if run loaduimage; then " \
		"echo Bootable SD/MMC-Card inserted, booting from it!; " \
		"run mmcboot; " \
	"else " \
		"echo ERROR: Unable to load uImage from SD/MMC-Card!; " \
		"panic; " \
	"fi; "

#elif defined CONFIG_FML80_ICT
/* ICT adapter boots only u-boot and does HW partitioning */
# define CONFIG_BOOTCOMMAND \
	"if mmc dev 0; mmc rescan; then " \
		"run sd_setup; " \
	"else " \
		"echo ERROR: SD/MMC-Card not detected!; " \
		"panic; " \
	"fi; " \
	"run fusecmd; "

#else /* !defined CONFIG__NETBOOT, !defined CONFIG_FML80_SDBOOT */
/* Regular Boot from internal eMMC */
# define CONFIG_BOOTCOMMAND \
	"if mmc dev 1; mmc rescan; then " \
		"run emmc_setup; " \
	"else " \
		"echo ERROR: eMMC device not detected!; " \
		"panic; " \
	"fi; " \
	"if run loaduimage; then " \
		"run mmcboot; " \
	"else " \
		"echo ERROR Unable to load uImage from eMMC!; " \
		"echo Performing Rollback!; " \
		"setenv _active_ ${active_root}; " \
		"setenv _inactive_ ${inactive_root}; " \
		"setenv active_root ${_inactive_}; " \
		"setenv inactive_root ${_active_}; " \
		"saveenv; " \
		"reset; " \
	"fi; "

#endif /* Regular Boot */

/* NS16550 Configuration */
#define CONFIG_SYS_NS16550_COM1		0x44e09000	/* UART0 */
#define CONFIG_SYS_NS16550_COM2		0x48022000	/* UART1 */
#define CONFIG_SYS_NS16550_COM3		0x48024000	/* UART2 */
#define CONFIG_SYS_NS16550_COM4		0x481a6000	/* UART3 */
#define CONFIG_SYS_NS16550_COM5		0x481a8000	/* UART4 */
#define CONFIG_SYS_NS16550_COM6		0x481aa000	/* UART5 */
#define CONFIG_CONS_INDEX               1

/* PMIC support */
#define CONFIG_POWER_TPS65217

/* SPL */

/*
 * Disable MMC DM for SPL build and can be re-enabled after adding
 * DM support in SPL
 */
#ifdef CONFIG_SPL_BUILD
#undef CONFIG_DM_MMC
#undef CONFIG_TIMER
#endif

#define CONFIG_CMD_DHCP
#define CONFIG_CMD_PING
#define CONFIG_DRIVER_TI_CPSW
#define CONFIG_RGMII
#define CONFIG_BOOTP_DEFAULT
#define CONFIG_BOOTP_DNS
#define CONFIG_BOOTP_DNS2
#define CONFIG_BOOTP_SEND_HOSTNAME
#define CONFIG_BOOTP_GATEWAY
#define CONFIG_BOOTP_SUBNETMASK
#define CONFIG_NET_RETRY_COUNT         10
#define CONFIG_PHY_ADDR			1
#define CONFIG_PHY_SMSC

/* I2C configuration */
#define CONFIG_SYS_I2C_EEPROM_ADDR	0x50	/* Main EEPROM */
#define CONFIG_SYS_I2C_EEPROM_ADDR_LEN	2
#define CONFIG_SYS_I2C_SPEED		400000
#define CONFIG_SYS_I2C_SLAVE		1

#define CONFIG_SHOW_BOOT_PROGRESS
#endif	/* ! __CONFIG_AM335X__H */
