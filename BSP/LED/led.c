#include "LED/led.h"



/**
 * @brief  初始化全部 LED GPIO (推挽输出, 默认熄灭)
 */
void led_init(void)
{
    GPIO_InitTypeDef gpio = {0};
    __HAL_RCC_GPIOG_CLK_ENABLE();


    /* 配 GPIO 为推挽输出, 默认低 (Active High → 默认灭) */
    gpio.Pin   = GPIO_PIN_10;
    gpio.Mode  = GPIO_MODE_OUTPUT_PP;
    gpio.Pull  = GPIO_NOPULL;
    gpio.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOG, &gpio);

    HAL_GPIO_WritePin(GPIOG, GPIO_PIN_10, GPIO_PIN_RESET);
}