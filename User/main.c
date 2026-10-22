#include "sys/sys.h"
#include "extmem_manager.h"
#include "HyperRAM/hyperram.h"
#include "XSPI/xspi.h"
#include "LED/led.h"

static HyperRAM_ObjectTypeDef HyperRAMObject = {0};

int main(void)
{
	Sys_Cache_Enable();                            /* 使能 I-Cache / D-Cache*/
	HAL_Init();                                    /* HAL 库初始化 */

	SystemClock_Config();                          /* 配置系统时钟:SMPS 供电 */
	PeriphCommonClock_Config();                    /* 配置外设时钟:USART1、LTDC 等 */
	XSPI1_Init();                                  /* 初始化 XSPI1 */
	XSPI2_Init();                                  /* 初始化 XSPI2 */
	EXTMEM_MANAGER_Init();                         /* 注册外部存储:EXTMEMORY_1 = NOR Flash(EXTMEM_USER 驱动) */

	/* HyperRAM 初始化:读 ID0/ID1 校验厂商与器件类型、解析容量、切换为差分时钟 */
	if (HyperRAM_OK != HyperRAM_Init(&HyperRAMObject, &hxspi1))
	{
		Error_Handler();                           
	}
    
	/* 使能 HyperRAM 内存映射模式,之后可像内部 RAM 一样直接访问 0x90000000 */
	if (HyperRAM_OK != HyperRAM_EnableMemoryMappedMode(&HyperRAMObject))
	{
		Error_Handler();
	}
	SystemIsolation_Config();                      /* TrustZone-M 安全隔离:RIMC + RISC + RISAF + GPIO + RCC */
    OpenDebug();                                   /* 打开调试端口(BSEC 调试授权) */
	if (BOOT_OK != BOOT_Application())
	{
		Error_Handler();
	}

	while (1)                                      
	{
	}
}

void HAL_MspInit(void)                             /* HAL 底层初始化回调,由 HAL_Init() 自动调用 */
{
  HAL_PWREx_EnableVddIO2();                        /* 使能 VDDIO2 电源域(XSPI1/HyperRAM 引脚所在 IO 域) */
  HAL_PWREx_ConfigVddIORange(PWR_VDDIO2, PWR_VDDIO_RANGE_1V8); /* VDDIO2 设为 1.8V,匹配 HyperRAM 电平 */

  HAL_PWREx_EnableVddIO3();                        /* 使能 VDDIO3 电源域(XSPI2/NOR Flash 引脚所在 IO 域) */
  HAL_PWREx_ConfigVddIORange(PWR_VDDIO3, PWR_VDDIO_RANGE_1V8); /* VDDIO3 设为 1.8V,匹配 NOR Flash 电平 */

  HAL_PWREx_EnableVddIO5();                        /* 使能 VDDIO5 电源域(LED 等 3.3V GPIO 所在 IO 域) */
  HAL_PWREx_ConfigVddIORange(PWR_VDDIO5, PWR_VDDIO_RANGE_3V3); /* VDDIO5 设为 3.3V */
}
