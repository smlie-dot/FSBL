#ifndef BSP_XSPI_INIT_H
#define BSP_XSPI_INIT_H

#include "stm32n6xx_hal.h"

#ifdef __cplusplus
extern "C" {
#endif

extern XSPI_HandleTypeDef hxspi1;   /* XSPI1 句柄(HyperRAM 使用) */
extern XSPI_HandleTypeDef hxspi2;   /* XSPI2 句柄(NOR Flash 使用) */

void XSPI1_Init(void);
void XSPI2_Init(void);

#ifdef __cplusplus
}
#endif

#endif