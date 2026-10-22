#ifndef __DELAY_H
#define __DELAY_H

#include "stm32n6xx_hal.h"

void delay_init(void);             /* 初始化 SysTick 延时, 内部读 SystemCoreClock */
void delay_us(uint32_t us);         /* 微秒延时, 直接读 SysTick 寄存器 */
void delay_ms(uint16_t ms);         /* 毫秒延时 (基于 delay_us) */

void HAL_Delay(uint32_t Delay);     /* 覆盖 HAL 弱定义, 内部走 delay_ms */

#endif
