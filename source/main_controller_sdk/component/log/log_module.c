#include "log_module.h"

#include "stm32h7xx_hal.h"
#include <stdio.h>

static log_ch_t* log_channel;

//easylogger port
void elog_port_output(const char *log, size_t size)
{
    if( log_channel != NULL )
    {
        log_channel->write( log_channel->ctx ,(uint8_t*)log,size);
    }
}

const char *elog_port_get_time(void)
{
    static char time_str[32];
    sprintf( time_str , "%08d" , (int)HAL_GetTick() );
    return time_str;
}

void log_module_buf_dump(const char* tag, uint8_t* buf , uint16_t len)
{
    uint8_t* cur_addr = buf;
    uint8_t* end_addr = buf + len;
    while( cur_addr < end_addr )
    {
        char str[64];
        sprintf(str,"%02X %02X %02X %02X %02X %02X %02X %02X" , cur_addr[0] , cur_addr[1] , cur_addr[2] , cur_addr[3] , cur_addr[4] , cur_addr[5] , cur_addr[6] , cur_addr[7] );
        LOG_DEBUG(tag,"%s",str);
        cur_addr += 8;
    }
}

void log_module_init(log_ch_t* log_ch)
{
    log_channel = log_ch;

    elog_init();

    elog_set_text_color_enabled(1);

    elog_set_fmt( ELOG_LVL_VERBOSE , ELOG_FMT_LVL | ELOG_FMT_TAG | ELOG_FMT_TIME );
    elog_set_fmt( ELOG_LVL_DEBUG , ELOG_FMT_LVL | ELOG_FMT_TAG | ELOG_FMT_TIME);
    elog_set_fmt( ELOG_LVL_INFO , ELOG_FMT_LVL | ELOG_FMT_TAG | ELOG_FMT_TIME);
    elog_set_fmt( ELOG_LVL_WARN , ELOG_FMT_LVL | ELOG_FMT_TAG | ELOG_FMT_TIME );
    elog_set_fmt( ELOG_LVL_ERROR , ELOG_FMT_LVL | ELOG_FMT_TAG | ELOG_FMT_TIME );

    elog_start();
}
