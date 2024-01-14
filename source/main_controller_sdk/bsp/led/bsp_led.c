#include "bsp_led.h"
#include "gpio.h"

int8_t bsp_led_init(bsp_led_t led)
{
    (void)led;
    return 0;
}

int8_t bsp_led_deinit(bsp_led_t led)
{
    (void)led;
    return 0;
}

void bsp_led_set(bsp_led_t index,uint16_t value)
{
    (void)index;
    HAL_GPIO_WritePin( GPIOE , GPIO_PIN_3 , value );
}
