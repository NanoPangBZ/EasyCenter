#include "bsp.h"
#include "gpio.h"
#include "usart.h"

static void peripheral_init(void)
{
    MX_GPIO_Init();
}

int8_t bsp_init(void)
{
    peripheral_init();

    bsp_led_init(DEBUG_LED);

    return 0;
}

int8_t bsp_deinit(void)
{
    bsp_led_deinit(DEBUG_LED);
    return 0;
}
