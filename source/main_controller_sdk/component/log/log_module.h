#pragma once

#include "./log/easylogger/inc/elog.h"

#define LOG_DEBUG(tag,...)      elog_d(tag, __VA_ARGS__);elog_flush()
#define LOG_INFO(tag,...)       elog_i(tag, __VA_ARGS__);elog_flush()
#define LOG_WARN(tag,...)       elog_w(tag, __VA_ARGS__);elog_flush()
#define LOG_ERR(tag,...)        elog_e(tag, __VA_ARGS__);elog_flush()

typedef struct
{
    void* ctx;
    uint16_t (*write)(void* ctx,uint8_t* data , uint16_t size);
}log_ch_t;

void log_module_init(log_ch_t* log_ch);

