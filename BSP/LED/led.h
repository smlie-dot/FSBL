#ifndef __LED_H
#define __LED_H

#include "sys/sys.h"

#define LED0_ON      HAL_GPIO_WritePin(GPIOG, GPIO_PIN_10, GPIO_PIN_SET)
#define LED0_OFF     HAL_GPIO_WritePin(GPIOG, GPIO_PIN_10, GPIO_PIN_RESET)
#define LED0_TOGGLE  HAL_GPIO_TogglePin(GPIOG, GPIO_PIN_10)

void led_init(void);                  /* 初始化全部 LED GPIO */


#endif /* __LED_H */