#include "bsp_qspi_flash.h"
#include "bsp_qspi_flash_log.h"
#include ".\w25qxx_qspi\w25qxx_qspi.h"

void bsp_qspi_flash_init(void)
{
    uint16_t flash_id;
    w25qxx_Init();
    flash_id = bsp_qspi_flash_get_id();
    BSP_QSPI_FLASH_INFO("qspi flash init , flash id:0x%04X." , flash_id);
}

void bsp_qspi_flash_deinit(void)
{
    //todo
}

uint16_t bsp_qspi_flash_get_id(void)
{
    return w25qxx_GetID();
}

int32_t bsp_qspi_flash_program(uint32_t addr,uint8_t* data,uint16_t len)
{
    BSP_QSPI_FLASH_INFO( "program 0x%04X-0x%04X , len:%d" , addr , addr + len , len );
    return W25qxx_PageProgram(data,addr,len);
}

int32_t bsp_qspi_flash_erase(uint32_t addr , uint16_t len)
{
    uint32_t cur_sector_addr = addr - addr % (4*1024);
    uint32_t end_sector_addr = (addr + len) - (addr + len) % (4*1024);

    BSP_QSPI_FLASH_INFO( "erease 0x%04X-0x%04X , len:%d" , cur_sector_addr , end_sector_addr + ( 4 * 1024 - 1 ) , (end_sector_addr -  cur_sector_addr + 4 * 1024) );

    while( cur_sector_addr <= end_sector_addr )
    {
        if( 0 != W25qxx_EraseSector(cur_sector_addr) )
        {
            BSP_QSPI_FLASH_ERR( "qspi flash erase sector(0x%04X) error." , cur_sector_addr );
            return -1;
        }
        cur_sector_addr += 4 * 1024;
    }

    return 0;
}

int32_t bsp_qspi_flash_full_erase(void)
{
    return W25qxx_EraseChip();
}

int32_t bsp_qspi_flash_write(uint32_t addr , uint8_t* data, uint16_t len)
{
    W25qxx_Write(data,addr,len);
    return 0;
}

int32_t bsp_qspi_flash_read(uint32_t addr , uint8_t* buf , uint16_t len)
{
    BSP_QSPI_FLASH_INFO( "read 0x%04X-0x%04X , len:%d" , addr , addr+len , len );
    return W25qxx_Read( buf , addr , len );
}
