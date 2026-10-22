#ifndef BSP_SYS_H
#define BSP_SYS_H

/* ---- HAL types needed for prototypes below ---- */
#include "stm32n6xx_hal.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ---- CMSE non-secure call macros (merged from FSBL/Core/Inc/main.h) ---- */
#if defined ( __ICCARM__ )
#  define CMSE_NS_CALL  __cmse_nonsecure_call
#  define CMSE_NS_ENTRY __cmse_nonsecure_entry
#else
#  define CMSE_NS_CALL  __attribute((cmse_nonsecure_call))
#  define CMSE_NS_ENTRY __attribute((cmse_nonsecure_entry))
#endif

/* ---- Function pointer types for non-secure callbacks (merged from main.h) ---- */
#if defined ( __ICCARM__ )
typedef void (CMSE_NS_CALL *funcptr)(void);
#else
typedef void CMSE_NS_CALL (*funcptr)(void);
#endif

typedef funcptr funcptr_NS;

/* ---- Public prototypes ---- */
void Error_Handler(void);
void SystemClock_Config(void);
void Mpu_Memory_Protection(void);
void PeriphCommonClock_Config(void);
void Sys_Cache_Enable(void);            /* I-Cache / D-Cache 使能 */
void OpenDebug(void);                   /* 打开调试端口(BSEC 调试授权) */
void SystemIsolation_Config(void);      /* TrustZone-M 安全隔离(RIMC + RISC + RISAF + GPIO + RCC) */

#ifdef __cplusplus
}
#endif

#endif /* BSP_SYS_H */