#pragma once

#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif  //__cplusplus

typedef void (*usb_cb_t)(void* ctx,uint8_t* data,uint16_t len);

int32_t bsp_usb_slave_init(void);
int32_t bsp_usb_write(uint8_t* buf , uint16_t len);
void bsp_usb_recieve_callback_register(usb_cb_t cb,void* ctx);

#ifdef __cplusplus
}
#endif  //__cplusplus
