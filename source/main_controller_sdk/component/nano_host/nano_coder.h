#pragma once

#include "nano_pack_define.h"

#ifdef __cplusplus
extern "C"
{
#endif  //__cplusplus

typedef struct nano_decoder_t* nano_decoder_handle_t;
typedef struct nano_encoder_t* nano_encoder_handle_t;

/*******************************************Nano解码器*******************************************************/

typedef enum{
    NANO_WAIT_BYTE = 0,
    NANO_DECODE_OK = 1,
    NANO_DECODE_ERR = 2,
}nano_decode_stage_t;

typedef void (*nano_decode_cb_t)(nano_decode_stage_t stage,nano_pack_t* pack,void* ctx);

nano_decoder_handle_t nano_decoder_static_create(uint8_t* static_buf,uint16_t buf_len);
int32_t nano_decoder_reset(nano_decoder_handle_t handle);
int32_t nano_decode_set_cb(nano_decoder_handle_t handle,nano_decode_cb_t cb,void* ctx);
void nano_decoder_input(nano_decoder_handle_t handle,uint8_t* bytes,uint16_t bytes_len,uint32_t curr_time);

/*******************************************Nano编码器*******************************************************/

nano_encoder_handle_t nano_encoder_static_create(uint8_t* static_buf,uint16_t buf_len);
uint32_t nano_encoder_output(nano_encoder_handle_t handle,nano_pack_t* pack,uint8_t* buf , uint16_t buf_len);

#ifdef __cplusplus
}
#endif  //__cplusplus
