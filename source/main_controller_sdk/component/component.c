#include "component.h"
#include "bsp.h"

static log_ch_t log_ch;

static uint16_t debug_port_write(void* ctx,uint8_t* data,uint16_t len)
{
    return bsp_uart_write( (bsp_uart_t)ctx , data , len );
}

int32_t component_init(void)
{
    //日志初始化
    log_ch.ctx = DEBUG_UART;
    log_ch.write = debug_port_write;
    log_module_init(&log_ch);

    return 0;
}
