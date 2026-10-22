cmake_minimum_required(VERSION 3.22)

#
# This file is generated only once,
# and is not re-generated if converter is called multiple times.
#
# User is free to modify the file as much as necessary
#

# Setup compiler settings
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)

# Define the build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()

message("Build type: " ${CMAKE_BUILD_TYPE})

# Enable compile command to ease indexing with e.g. clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

# Compiler options
set(STM32_MCU_FLAGS  "-mcpu=cortex-m55 -mfpu=fpv5-d16 -mfloat-abi=hard -mcmse " )

#Linker options
set(STM32_LINKER_SCRIPT boot/STM32N647XX_AXISRAM2_fsbl.ld)
set(STM32_LINKER_OPTION  )

# Set the project name
set(CMAKE_PROJECT_NAME demo_FSBL)
project(${CMAKE_PROJECT_NAME})

# Create an executable object type
add_executable(${CMAKE_PROJECT_NAME})

# In order to use CMake for cross-compiling
set(CMAKE_CXX_COMPILER_FORCED "true")

# Add project symbols (macros)
target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE
    USE_HAL_DRIVER
    STM32N647xx
)

# Enable CMake support for ASM and C languages
enable_language(C ASM)

# Auto-discover user-side sources (re-runs on every build to catch new files).
# HAL driver .c files are NOT included via GLOB: the HAL ships *_template.c
# files (timebase / MSP) that are not meant to be compiled as-is, plus many
# per-module drivers that pull in headers from disabled modules. Drivers are
# listed explicitly below.
set(FSBL_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/User/extmem_manager.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/User/system_stm32n6xx_fsbl.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/User/stm32n6xx_it.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/User/main.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/Middlewares/ST/STM32_ExtMem_Manager/boot/stm32_boot_xip.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/Middlewares/ST/STM32_ExtMem_Manager/nor_sfdp/stm32_sfdp_data.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/Middlewares/ST/STM32_ExtMem_Manager/nor_sfdp/stm32_sfdp_driver.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/Middlewares/ST/STM32_ExtMem_Manager/psram/stm32_psram_driver.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/Middlewares/ST/STM32_ExtMem_Manager/sal/stm32_sal_sd.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/Middlewares/ST/STM32_ExtMem_Manager/sal/stm32_sal_xspi.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/Middlewares/ST/STM32_ExtMem_Manager/sdcard/stm32_sdcard_driver.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/Middlewares/ST/STM32_ExtMem_Manager/stm32_extmem.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/Middlewares/ST/STM32_ExtMem_Manager/user/stm32_user_driver.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/boot/startup_stm32n647xx_fsbl.s"
)

# STM32 HAL drivers actually needed by the FSBL image
# (matches the modules enabled in Core/sys/stm32n6xx_hal_conf.h:
#  RCC, GPIO, DMA, PWR, CORTEX, EXTI, XSPI).
#  RIF added for SystemIsolation_Config() (RIMC/RISC/RISAF 配置搬到了 FSBL).
set(FSBL_HAL_DRIVERS
    "${CMAKE_CURRENT_SOURCE_DIR}/Drivers/STM32N6xx_HAL_Driver/Src/stm32n6xx_hal.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/Drivers/STM32N6xx_HAL_Driver/Src/stm32n6xx_hal_rcc.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/Drivers/STM32N6xx_HAL_Driver/Src/stm32n6xx_hal_rcc_ex.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/Drivers/STM32N6xx_HAL_Driver/Src/stm32n6xx_hal_gpio.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/Drivers/STM32N6xx_HAL_Driver/Src/stm32n6xx_hal_dma.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/Drivers/STM32N6xx_HAL_Driver/Src/stm32n6xx_hal_dma_ex.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/Drivers/STM32N6xx_HAL_Driver/Src/stm32n6xx_hal_pwr.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/Drivers/STM32N6xx_HAL_Driver/Src/stm32n6xx_hal_pwr_ex.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/Drivers/STM32N6xx_HAL_Driver/Src/stm32n6xx_hal_cortex.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/Drivers/STM32N6xx_HAL_Driver/Src/stm32n6xx_hal_exti.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/Drivers/STM32N6xx_HAL_Driver/Src/stm32n6xx_hal_xspi.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/Drivers/STM32N6xx_HAL_Driver/Src/stm32n6xx_hal_bsec.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/Drivers/STM32N6xx_HAL_Driver/Src/stm32n6xx_hal_rif.c"
)

# Board-support drivers. This project is self-contained: BSP/Core/Drivers/
# Middlewares are vendored under the project root, so no paths escape it.
set(FSBL_BSP_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/BSP/HyperRAM/hyperram.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/BSP/LED/led.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/BSP/NORFlash/extmem_user_driver.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/BSP/NORFlash/norflash.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/BSP/NORFlash/norflash_by25fq128el.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/BSP/NORFlash/norflash_mx25um25645g.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/BSP/NORFlash/norflash_xspi.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/Core/XSPI/xspi.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/Core/sys/sys.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/Core/sys/syscalls.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/Core/sys/sysmem.c"
)

# Add include paths
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/User"
    "${CMAKE_CURRENT_SOURCE_DIR}/Drivers/STM32N6xx_HAL_Driver/Inc"
    "${CMAKE_CURRENT_SOURCE_DIR}/Drivers/STM32N6xx_HAL_Driver/Inc/Legacy"
    "${CMAKE_CURRENT_SOURCE_DIR}/Drivers/CMSIS/Device/ST/STM32N6xx/Include"
    "${CMAKE_CURRENT_SOURCE_DIR}/Drivers/CMSIS/Include"
    "${CMAKE_CURRENT_SOURCE_DIR}/Middlewares/ST/STM32_ExtMem_Manager"
    "${CMAKE_CURRENT_SOURCE_DIR}/Middlewares/ST/STM32_ExtMem_Manager/user"
    "${CMAKE_CURRENT_SOURCE_DIR}/BSP"
    "${CMAKE_CURRENT_SOURCE_DIR}/BSP/HyperRAM"
    "${CMAKE_CURRENT_SOURCE_DIR}/BSP/NORFlash"
    "${CMAKE_CURRENT_SOURCE_DIR}/Core"
    "${CMAKE_CURRENT_SOURCE_DIR}/Core/sys"
)

# Add sources to executable
target_sources(${CMAKE_PROJECT_NAME} PRIVATE ${FSBL_SOURCES} ${FSBL_HAL_DRIVERS} ${FSBL_BSP_SOURCES})

# Link directories setup
target_link_directories(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined library search paths
)

# Add linked libraries
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined libraries
)

# Remove wrong libob.a library dependency when using cpp files
list(REMOVE_ITEM CMAKE_C_IMPLICIT_LINK_LIBRARIES ob)

# Generate .bin from ELF (CubeMX template omits this by default;
# needed for STM32_SigningTool_CLI / STM32CubeProgrammer flashing).
add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O binary
        $<TARGET_FILE:${CMAKE_PROJECT_NAME}>
        ${CMAKE_PROJECT_NAME}.bin
    COMMENT "Building ${CMAKE_PROJECT_NAME}.bin"
)

# Validate that STM32CubeMX code is compatible with C standard
if((CMAKE_C_STANDARD EQUAL 90) OR (CMAKE_C_STANDARD EQUAL 99))
    message(ERROR "Generated code requires C11 or higher")
endif()








