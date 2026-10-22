/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    stm32_extmem_conf.h
  * @brief   External memory configuration — NOR Flash is managed through
  *           the EXTMEM_USER driver (NORFlash BSP at
  *           Drivers/BSP/NORFlash/extmem_user_driver.c, driving XSPI2 with
  *           Macronix MX25UM25645G). The application image is stored in the
  *           NOR Flash and XIPed from there by BOOT_Application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

#ifndef __STM32_EXTMEM_CONF__H__
#define __STM32_EXTMEM_CONF__H__

#ifdef __cplusplus
 extern "C" {
#endif

/* Driver layer enable --------------------------------------------------------*/
#define EXTMEM_DRIVER_NOR_SFDP   0   /* NOR Flash (SFDP) driver — disabled */
#define EXTMEM_DRIVER_PSRAM      0   /* PSRAM driver — disabled */
#define EXTMEM_DRIVER_SDCARD     0   /* SD card driver — disabled */
#define EXTMEM_DRIVER_USER       1   /* NORFlash BSP via EXTMEM_USER hooks */

/* SAL (Serial-Abstraction Layer) enable --------------------------------------*/
#define EXTMEM_SAL_XSPI          0
#define EXTMEM_SAL_SD            0

/* Includes ------------------------------------------------------------------*/
#include "stm32n6xx_hal.h"
#include "stm32_extmem.h"
#include "stm32_extmem_type.h"
#include "boot/stm32_boot_xip.h"

/* USER CODE BEGIN INCLUDE */

/* USER CODE END INCLUDE */

/* Exported constants --------------------------------------------------------*/
enum {
  EXTMEMORY_1  = 0   /*!< ID=0: NOR Flash (Macronix MX25UM25645G) on XSPI2 */
};

/* Boot XIP configuration ----------------------------------------------------*/
#define EXTMEM_MEMORY_BOOTXIP     EXTMEMORY_1
#define EXTMEM_XIP_IMAGE_OFFSET   0x100000U   /* Application image at 1 MB offset */
#define EXTMEM_HEADER_OFFSET      0x400U      /* Vector table at 1 KB offset within the image */

extern EXTMEM_DefinitionTypeDef extmem_list_config[1];
#if defined(EXTMEM_C)
EXTMEM_DefinitionTypeDef extmem_list_config[1];
#endif

#ifdef __cplusplus
}
#endif

#endif /* __STM32_EXTMEM_CONF__H__ */
