#include "delay.h"

/* us 延时倍乘系数: 每 us 对应 SysTick 计数次数 = CPU MHz */
static uint32_t g_fac_us = 0;

/**
 * @brief  初始化 SysTick 延时
 * @note   直接读 SysTick 寄存器, 与参考工程 (实验1 跑马灯实验) 裸机路径一致
 * @note   必须在 SystemClock_Config() 之后调用, 以便 SystemCoreClock 已是 CPU 实际频率
 */
void delay_init(void)
{
    g_fac_us = SystemCoreClock / 1000000U;  /* 每 us 对应 SysTick 计数次数 = CPU MHz */
}

/**
 * @brief     微秒延时, 直接读 SysTick->VAL 寄存器累加差值
 * @note      与参考工程 delay_us 实现 1:1
 * @param     us : 要延时的微秒数
 */
void delay_us(uint32_t us)
{
    uint32_t ticks;
    uint32_t told, tnow, tcnt = 0;
    uint32_t reload = SysTick->LOAD;                    /* LOAD 的值 */
    ticks = us * g_fac_us;                              /* 需要的计数次数 */

    told = SysTick->VAL;                                /* 进入时计数器值 */
    while (1)
    {
        tnow = SysTick->VAL;
        if (tnow != told)
        {
            if (tnow < told)
            {
                tcnt += told - tnow;                    /* 向下计数, 直接相减 */
            }
            else
            {
                tcnt += reload - tnow + told;           /* 计数器回绕, 补上 reload */
            }
            told = tnow;
            if (tcnt >= ticks)
            {
                break;                                  /* 时间达到, 退出 */
            }
        }
    }
}

/**
 * @brief  毫秒延时, 直接通过 delay_us 实现
 * @param  ms : 要延时的毫秒数 (0 < nms <= 65535)
 */
void delay_ms(uint16_t ms)
{
    delay_us((uint32_t)ms * 1000);
}


