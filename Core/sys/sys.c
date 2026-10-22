#include "stm32n6xx_hal.h"
#include "sys.h"

/**
 * @brief  I-Cache / D-Cache 使能 (FSBL/Appli 共用)
 */
void Sys_Cache_Enable(void)
{
  SCB_EnableICache();
  SCB_EnableDCache();
}



/**
 * @brief  打开调试端口(BSEC 调试授权)
 * @note   需在 HAL_Init() 之后、跳转到应用之前调用
 */
void OpenDebug(void)
{
  BSEC_HandleTypeDef hbsec = {0};
  BSEC_DebugCfgTypeDef config_debug = {0};

  hbsec.Instance = BSEC;

  config_debug.HDPL_Open_Dbg   = HAL_BSEC_OPEN_DBG_LEVEL_0;  /* 从 HDPL0 起即允许调试 */
  config_debug.Sec_Dbg_Auth    = HAL_BSEC_SEC_DBG_AUTH;      /* 授权安全侧调试 */
  config_debug.NonSec_Dbg_Auth = HAL_BSEC_NONSEC_DBG_AUTH;   /* 授权非安全侧调试 */

  if (HAL_BSEC_ConfigDebug(&hbsec, &config_debug) != HAL_OK)
  {
    Error_Handler();
  }

  if (HAL_BSEC_UnlockDebug(&hbsec) != HAL_OK)
  {
    Error_Handler();
  }
}


void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /* 配置系统电源供电方式:使用 SMPS 开关电源 */
  if (HAL_PWREx_ConfigSupply(PWR_SMPS_SUPPLY) != HAL_OK)
  {
    Error_Handler();
  }

  /* 使能 HSI(内部高速时钟,64MHz),作为切换过程中的中间时钟;此步不启用任何 PLL */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSIDiv = RCC_HSI_DIV1;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL1.PLLState = RCC_PLL_NONE;
  RCC_OscInitStruct.PLL2.PLLState = RCC_PLL_NONE;
  RCC_OscInitStruct.PLL3.PLLState = RCC_PLL_NONE;
  RCC_OscInitStruct.PLL4.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /* 等待 HSE 起振稳定,之后才能作为 PLL 的时钟源 */
  HAL_Delay(HSE_STARTUP_TIMEOUT);

  /* 读取当前 CPU/系统总线时钟配置;若正跑在 IC 时钟上,先切回 HSI 这个中间时钟,
     以保证后续能安全地重新配置目标时钟 */
  HAL_RCC_GetClockConfig(&RCC_ClkInitStruct);
  if ((RCC_ClkInitStruct.CPUCLKSource == RCC_CPUCLKSOURCE_IC1) ||
     (RCC_ClkInitStruct.SYSCLKSource == RCC_SYSCLKSOURCE_IC2_IC6_IC11))
  {
    RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_CPUCLK | RCC_CLOCKTYPE_SYSCLK);
    RCC_ClkInitStruct.CPUCLKSource = RCC_CPUCLKSOURCE_HSI;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct) != HAL_OK)
    {
      /* 初始化失败 */
      Error_Handler();
    }
  }

  /* 按 RCC_OscInitTypeDef 中的参数配置振荡器:启用 HSE 并配置 PLL1~PLL4 */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;

  /* PLL 计算公式:VCO = HSE / PLLM * PLLN,输出 = VCO / (PLLP1 * PLLP2),HSE = 48MHz */

  /* PLL1: 48/3=16MHz 参考,VCO=16*50=800MHz,输出=800/(1*1) = 800MHz */
  RCC_OscInitStruct.PLL1.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL1.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL1.PLLM = 3;
  RCC_OscInitStruct.PLL1.PLLN = 50;
  RCC_OscInitStruct.PLL1.PLLFractional = 0;
  RCC_OscInitStruct.PLL1.PLLP1 = 1;
  RCC_OscInitStruct.PLL1.PLLP2 = 1;

  /* PLL2: 48/3=16MHz 参考,VCO=16*125=2000MHz,输出=2000/(2*1) = 1000MHz */
  RCC_OscInitStruct.PLL2.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL2.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL2.PLLM = 3;
  RCC_OscInitStruct.PLL2.PLLN = 125;
  RCC_OscInitStruct.PLL2.PLLFractional = 0;
  RCC_OscInitStruct.PLL2.PLLP1 = 2;
  RCC_OscInitStruct.PLL2.PLLP2 = 1;

  /* PLL3: 48/4=12MHz 参考,VCO=12*75=900MHz,输出=900/(1*1) = 900MHz */
  RCC_OscInitStruct.PLL3.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL3.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL3.PLLM = 4;
  RCC_OscInitStruct.PLL3.PLLN = 75;
  RCC_OscInitStruct.PLL3.PLLFractional = 0;
  RCC_OscInitStruct.PLL3.PLLP1 = 1;
  RCC_OscInitStruct.PLL3.PLLP2 = 1;

  /* PLL4: 48/1=48MHz 参考,VCO=48*25=1200MHz,输出=1200/(1*1) = 1200MHz */
  RCC_OscInitStruct.PLL4.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL4.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL4.PLLM = 1;
  RCC_OscInitStruct.PLL4.PLLN = 25;
  RCC_OscInitStruct.PLL4.PLLFractional = 0;
  RCC_OscInitStruct.PLL4.PLLP1 = 1;
  RCC_OscInitStruct.PLL4.PLLP2 = 1;

  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /* 配置 CPU、AHB、APB 各总线时钟 */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_CPUCLK|RCC_CLOCKTYPE_HCLK
                              |RCC_CLOCKTYPE_SYSCLK|RCC_CLOCKTYPE_PCLK1
                              |RCC_CLOCKTYPE_PCLK2|RCC_CLOCKTYPE_PCLK5
                              |RCC_CLOCKTYPE_PCLK4;
  RCC_ClkInitStruct.CPUCLKSource = RCC_CPUCLKSOURCE_IC1;       /* CPU 时钟源 = IC1 -> 800MHz */
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_IC2_IC6_IC11; /* 系统总线时钟源 = IC2/IC6/IC11 组 */
  RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV2;             /* HCLK = SYSCLK(400MHz) / 2 = 200MHz */
  RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV1;            /* PCLK1 = HCLK / 1 = 200MHz */
  RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV1;            /* PCLK2 = HCLK / 1 = 200MHz */
  RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV1;            /* PCLK4 = HCLK / 1 = 200MHz */
  RCC_ClkInitStruct.APB5CLKDivider = RCC_APB5_DIV1;            /* PCLK5 = HCLK / 1 = 200MHz */
  RCC_ClkInitStruct.IC1Selection.ClockSelection = RCC_ICCLKSOURCE_PLL1;
  RCC_ClkInitStruct.IC1Selection.ClockDivider = 1;             /* IC1  = PLL1(800MHz)  / 1 = 800MHz -> CPUCLK */
  RCC_ClkInitStruct.IC2Selection.ClockSelection = RCC_ICCLKSOURCE_PLL1;
  RCC_ClkInitStruct.IC2Selection.ClockDivider = 2;             /* IC2  = PLL1(800MHz)  / 2 = 400MHz -> SYSCLK/AXI */
  RCC_ClkInitStruct.IC6Selection.ClockSelection = RCC_ICCLKSOURCE_PLL2;
  RCC_ClkInitStruct.IC6Selection.ClockDivider = 1;             /* IC6  = PLL2(1000MHz) / 1 = 1000MHz */
  RCC_ClkInitStruct.IC11Selection.ClockSelection = RCC_ICCLKSOURCE_PLL3;
  RCC_ClkInitStruct.IC11Selection.ClockDivider = 1;            /* IC11 = PLL3(900MHz)  / 1 = 900MHz */

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
}


void PeriphCommonClock_Config(void)
{
    RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

    /* USART1: IC14 ← PLL3(900MHz) ÷9 = 100MHz */
    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_USART1;
    PeriphClkInitStruct.Usart1ClockSelection = RCC_USART1CLKSOURCE_IC14;
    PeriphClkInitStruct.ICSelection[RCC_IC14].ClockSelection = RCC_ICCLKSOURCE_PLL3;
    PeriphClkInitStruct.ICSelection[RCC_IC14].ClockDivider = 9;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
    {
      Error_Handler();
    }

    /* LTDC: IC16 ← PLL4(1200MHz) ÷50 = 24MHz */
    PeriphClkInitStruct.PeriphClockSelection |= RCC_PERIPHCLK_LTDC;
    PeriphClkInitStruct.LtdcClockSelection    = RCC_LTDCCLKSOURCE_IC16;
    PeriphClkInitStruct.ICSelection[RCC_IC16].ClockSelection = RCC_ICCLKSOURCE_PLL4;
    PeriphClkInitStruct.ICSelection[RCC_IC16].ClockDivider   = 50;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
    {
      Error_Handler();
    }
}

/**
 * @brief      设置指定区域的MPU保护
 * @param      baseaddr:MPU保护区的基址(首地址)
 * @param      size:MPU保护区的尺寸(必须是32的倍数,单位为字节),参数可参考:CORTEX_MPU_Region_Size
 * @param      rnum:MPU保护区号,范围:0~7,最多支持8个保护区,参数可参考:CORTEX_MPU_Region_Number
 * @param      de:禁止指令执行;0,允许指令执行;1,禁止指令执行
 * @param      ap:访问权限,读/写关系,参数可参考:CORTEX_MPU_Region_Permission_Attributes
 *   @arg     MPU_REGION_NO_ACCESS,无访问(特权&用户都不可访问)
 *   @arg     MPU_REGION_PRIV_RW,仅支持特权读写
 *   @arg     MPU_REGION_PRIV_RW_URO,禁止用户写访问，特权可读写访问
 *   @arg     MPU_REGION_FULL_ACCESS,全访问(特权&用户都可访问)
 *   @arg     MPU_REGION_PRIV_RO,仅支持特权读取
 *   @arg     MPU_REGION_PRIV_RO_URO,只允许特权&用户读，不可写
 *   @arg     参考:STM32F7参考手册.pdf,4.6节,Table 89.
 * @param      sen:是否共享;MPU_ACCESS_NOT_SHAREABLE,非共享;MPU_ACCESS_SHAREABLE,共享
 * @param      cen:是否使能cache;MPU_ACCESS_NOT_CACHEABLE,不使能;MPU_ACCESS_CACHEABLE,使能
 * @param      ben:是否使能缓冲;MPU_ACCESS_NOT_BUFFERABLE,不使能;MPU_ACCESS_BUFFERABLE,使能
 * @retval     0,成功.
 *             其他,失败.
 */
/**
 * @brief      只保护外部 HyperRAM（挂在 XSPI1 上，0x90000000，长度 8M，与链接脚本 EXTRAM 一致）
 * @note       STM32N6 是 PMSAv8，cache/buffer 通过 MAIR 属性配置（AttributesIndex）。
 *             这里配置为 Normal-Non-Cacheable（INNER/OUTER 均不使能 cache）。
 */
void Mpu_Memory_Protection(void)
{
    MPU_Attributes_InitTypeDef mpu_attr   = {0};
    MPU_Region_InitTypeDef    mpu_region = {0};

    /* MAIR[0] = Normal memory, Non-Cacheable（inner & outer 都不使能 cache） */
    mpu_attr.Number     = MPU_ATTRIBUTES_NUMBER0;
    mpu_attr.Attributes = INNER_OUTER(MPU_NOT_CACHEABLE);
    HAL_MPU_ConfigMemoryAttributes(&mpu_attr);

    /* 外部 HyperRAM (XSPI1 memory-mapped): 0x90000000 ~ 0x907FFFFF (8MB) */
    mpu_region.Enable           = MPU_REGION_ENABLE;
    mpu_region.Number           = MPU_REGION_NUMBER0;
    mpu_region.AttributesIndex  = MPU_ATTRIBUTES_NUMBER0;
    mpu_region.BaseAddress      = 0x90000000U;
    mpu_region.LimitAddress     = 0x90000000U + 8U * 1024U * 1024U - 1U;   /* 0x907FFFFF */
    mpu_region.AccessPermission = MPU_REGION_ALL_RW;                       /* 特权/非特权都可读写 */
    mpu_region.DisableExec      = MPU_INSTRUCTION_ACCESS_DISABLE;          /* 禁止取指 */
    mpu_region.DisablePrivExec  = MPU_PRIV_INSTRUCTION_ACCESS_DISABLE;     /* 特权下也禁止取指 */
    mpu_region.IsShareable      = MPU_ACCESS_NOT_SHAREABLE;
    HAL_MPU_ConfigRegion(&mpu_region);

    HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);
}


/**
 * @brief  TrustZone-M 安全隔离 (最小化, 对齐 ST LTDC_YUV_Full_Planar 例程 main.c:280-285)
 * @note   例程默认 CPU 跑在 SEC|PRIV, 复位态 RIF 已放行 LTDC, 这里只显式确认一次.
 *         不再做: NSEC 推广 / GPIO 属性 / RCC 提前使能 / DMA2D-XSPI 推广.
 *         新增: RISAF1 (internal SRAM 0x24000000~0x24400000) 开 CID1 RW,
 *               否则 DMA2D 写帧缓冲被静默拒 (CPU/LTDC 默认放行看不出问题).
 *         MPU (Mpu_Memory_Protection) 与本函数无关, 例程也没 MPU, 这里保留.
 *         适用场景: 短期放弃 TrustZone 排查或确认 secure world 下行为.
 */
void SystemIsolation_Config(void)
{
    /* RIMC Master — LTDC1 master 沿用默认 CID1, 显式钉死 SEC|PRIV (与例程一致) */
    RIMC_MasterConfig_t RIMC_master = { 0 };
    RIMC_master.MasterCID = RIF_CID_1;
    RIMC_master.SecPriv   = RIF_ATTRIBUTE_SEC | RIF_ATTRIBUTE_PRIV;
    HAL_RIF_RIMC_ConfigMasterAttributes(RIF_MASTER_INDEX_LTDC1, &RIMC_master);

    /* RISC Slave — LTDC slave SEC|PRIV (与例程 main.c:285 一致) */
    HAL_RIF_RISC_SetSlaveSecureAttributes(RIF_RISC_PERIPH_INDEX_LTDCL1,
                                          RIF_ATTRIBUTE_SEC | RIF_ATTRIBUTE_PRIV);

    /* RIMC Master — DMA2D master: 同样放行, 否则 DMA2D 写不进帧缓冲 (RISAF 按 CID 过滤) */
    HAL_RIF_RIMC_ConfigMasterAttributes(RIF_MASTER_INDEX_DMA2D, &RIMC_master);

    /* RISC Slave — DMA2D slave: 同样放行, 否则 Appli 配置不了 DMA2D 外设寄存器 */
    HAL_RIF_RISC_SetSlaveSecureAttributes(RIF_RISC_PERIPH_INDEX_DMA2D,
                                          RIF_ATTRIBUTE_SEC | RIF_ATTRIBUTE_PRIV);

    /* RISAF1 (internal SRAM) — DMA2D 写帧缓冲的最后一道关.
     *   RISAF 按 region (CID 白名单) 过滤; 不开的话 DMA2D 写 .bss 帧缓冲会被静默拒,
     *   CPU 写入没影响是因为 CPU 的 CID 与 LTDC 默认在白名单里, 但 DMA2D 不是.
     *   已把 DMA2D 的 RIMC MasterCID 钉到 CID1 (上面), 所以这里 CID1 RW 就够. */
    {
        RISAF_BaseRegionConfig_t base = {0};
        base.Filtering      = RISAF_FILTER_ENABLE;
        base.Secure         = RIF_ATTRIBUTE_SEC;            /* CPU 在 SEC 域 (CPU_IN_SECURE_STATE) */
        base.PrivWhitelist  = RIF_CID_1;
        base.ReadWhitelist  = RIF_CID_1;
        base.WriteWhitelist = RIF_CID_1;                    /* DMA2D 写帧缓冲的关键 */
        base.StartAddress   = 0x24000000U;                 /* N647 SRAM 起点 (4 KB 对齐) */
        base.EndAddress     = 0x243FFFFFU;                 /* 4 MB - 1 */
        HAL_RIF_RISAF_ConfigBaseRegion(RISAF1, RISAF_REGION_1, &base);
    }

    /* MPU + RISAF3 — 帧缓冲搬到外部 HyperRAM (XSPI1, 0x90000000) 后的两条腿:
     *   MPU  把 0x90000000~0x907FFFFF 标为 Normal Non-Cacheable,
     *          让 CPU 写进 HyperRAM 不走 D-Cache, LTDC 下一帧能直接看到;
     *          (此前帧缓冲在 SRAM 时必须 SCB_CleanDCache_by_Addr, 现在免了)
     *   RISAF3  CID1 RW 白名单: 不开则 DMA2D 静默拒访. 地址用相对 XSPI1 base 的偏移,
     *             StartAddress=0, EndAddress=0xFFFFF (限 1MB, 768KB 帧缓冲够). */
    Mpu_Memory_Protection();
    {
        RISAF_BaseRegionConfig_t base = {0};
        base.Filtering      = RISAF_FILTER_ENABLE;
        base.Secure         = RIF_ATTRIBUTE_SEC;
        base.PrivWhitelist  = RIF_CID_1;
        base.ReadWhitelist  = RIF_CID_1;
        base.WriteWhitelist = RIF_CID_1;                    /* DMA2D 写外 RAM 的关键 */
        base.StartAddress   = 0x00000000U;                 /* 偏移 0 → 物理 0x90000000 */
        base.EndAddress     = 0x000FFFFFU;                 /* 1 MB region (4 KB 对齐) */
        HAL_RIF_RISAF_ConfigBaseRegion(RISAF3, RISAF_REGION_1, &base);
    }
}


void Error_Handler(void)
{
  __disable_irq();
  while (1)
  {
  }
}