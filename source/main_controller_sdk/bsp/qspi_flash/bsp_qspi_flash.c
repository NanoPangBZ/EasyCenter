#include "bsp_qspi_flash.h"
#include "quadspi.h"

void bsp_qspi_flash_init(qspi_flash_mode_t mode)
{
    MX_QUADSPI_Init();
}

void bsp_qspi_flash_deinit(void)
{

}

uint16_t bsp_qspi_flash_get_id(void)
{
    return 0;
}

int32_t bsp_qspi_flash_program(uint32_t addr,uint8_t* data,uint16_t len)
{
    return -1;
}

int32_t bsp_qspi_flash_erase(uint32_t addr , uint16_t len)
{
    return -1;
}


