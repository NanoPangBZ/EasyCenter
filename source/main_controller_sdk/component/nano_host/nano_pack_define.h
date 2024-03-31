#pragma once

#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

//pack中所包含的信息
typedef struct{
    uint16_t cmd;                   //命令
    uint8_t pack_id;                //包ID
    uint16_t reserved;              //未定义预留
    uint8_t need_ack:1;             //是否需要应答
    uint8_t is_ack:1;               //这是一个应答包
    const uint8_t* payload_data;    //负载数据
    uint16_t payload_len;           //负载数据长度
}nano_pack_t;

#ifdef __cplusplus
}
#endif // __cplusplus

