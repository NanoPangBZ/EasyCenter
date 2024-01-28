#pragma once

#define BSP_QSPI_FLASH_LOG_ENABLE   (1)

#if BSP_QSPI_FLASH_LOG_ENABLE
    #include "./log/log_module.h"
    #define TAG "exFlash"
    #define BSP_QSPI_FLASH_DEBUG(...)   LOG_DEBUG(TAG,__VA_ARGS__)
    #define BSP_QSPI_FLASH_INFO(...)    LOG_INFO(TAG,__VA_ARGS__)
    #define BSP_QSPI_FLASH_WARN(...)    LOG_WARN(TAG,__VA_ARGS__)
    #define BSP_QSPI_FLASH_ERR(...)     LOG_ERR(TAG,__VA_ARGS__)
#else
    #define BSP_QSPI_FLASH_DEBUG(...)
    #define BSP_QSPI_FLASH_INFO(...)
    #define BSP_QSPI_FLASH_WARN(...)
    #define BSP_QSPI_FLASH_ERR(...)
#endif

