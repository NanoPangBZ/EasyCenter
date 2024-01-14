#pragma once

#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif  //__cplusplus

typedef enum{
    DEBUG_LED = 0,
}bsp_led_t;

int8_t bsp_led_init(bsp_led_t led);
int8_t bsp_led_deinit(bsp_led_t led);
void bsp_led_set(bsp_led_t index,uint16_t value);

#ifdef __cplusplus
}
#endif  //__cplusplus