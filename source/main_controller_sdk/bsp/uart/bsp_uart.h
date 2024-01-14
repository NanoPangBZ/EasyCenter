#pragma once

#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif  //__cplusplus

typedef enum{
    DEBUG_UART = 0,
}bsp_uart_t;

int8_t bsp_uart_init(bsp_uart_t index);
int8_t bsp_uart_deinit(bsp_uart_t index);
int16_t bsp_uart_write(bsp_uart_t index , uint8_t* buf , uint16_t len);
int16_t bsp_uart_read(bsp_uart_t index , uint8_t* buf , uint16_t len);

#ifdef __cplusplus
}
#endif  //__cplusplus


