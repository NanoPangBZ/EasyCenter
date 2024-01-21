#pragma once

#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif  //__cplusplus

typedef enum{
    QSPI_MODE = 0,
    XIP_MODE = 1
}qspi_flash_mode_t;

void bsp_qspi_flash_init(qspi_flash_mode_t mode);
void bsp_qspi_flash_deinit(void);
uint16_t bsp_qspi_flash_get_id(void);
int32_t bsp_qspi_flash_program(uint32_t addr,uint8_t* data,uint16_t len);
int32_t bsp_qspi_flash_erase(uint32_t addr , uint16_t len);

#ifdef __cplusplus
}
#endif  //__cplusplus