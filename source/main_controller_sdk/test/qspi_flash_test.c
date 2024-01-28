#include "qspi_flash_test.h"
#include "./qspi_flash/bsp_qspi_flash.h"
#include "./log/log_module.h"
#include <stdlib.h>
#include <string.h>

#define TAG "test"

int32_t qspi_flash_test(void)
{
    uint8_t restore_buf[4*1024];
    uint8_t test_buf[4*1024];
    uint8_t test_buf_2[4*1024];
    uint16_t test_size = 4 * 1024;
    uint32_t test_addr = 4 * 1024;

    // bsp_qspi_flash_init();
    bsp_qspi_flash_read(test_addr,restore_buf,test_size);

    //ID读取
    uint16_t qspi_flash_id = bsp_qspi_flash_get_id();
    LOG_INFO(TAG,"get qspi flash id:0x%04X" , qspi_flash_id);

    //擦除测试
    bsp_qspi_flash_erase(test_addr,test_size);
    bsp_qspi_flash_read(test_addr,test_buf,test_size);
    for( uint16_t temp = 0 ; temp < test_size ; temp++)
    {
        if( test_buf[temp] != 0xff )
        {
            LOG_ERR(TAG,"ExFlash erase test failed! Error addr:0x%04X , flash data:0x%02X" , test_addr + temp , test_buf[temp] );
            return -1;
        }
    }
    LOG_INFO(TAG,"ExFlash erase test success!");

    //写入测试
    for( uint16_t temp = 0 ; temp < test_size ; temp++)
    {
        test_buf[temp] = rand();
    }
    bsp_qspi_flash_write(test_addr,test_buf,test_size);
    bsp_qspi_flash_read(test_addr,test_buf_2,test_size);
    for( uint16_t temp = 0 ; temp < test_size ; temp++ )
    {
        if( test_buf[temp] != test_buf_2[temp] )
        {
            LOG_ERR(TAG,"ExFlash write test failed! Error addr:0x%04X , write data:0x%02X , flash data:0x%02X" , test_addr + temp , test_buf[temp] , test_buf_2[temp]);
            return -1;
        }
    }
    LOG_INFO(TAG,"ExFlash write test success!");

    //restore原始数据
    bsp_qspi_flash_write(test_addr,restore_buf,test_size);

    return 0;
}


