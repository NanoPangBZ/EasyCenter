#include "bsp.h"
#include "gpio.h"

static void peripheral_init(void)
{
    MX_GPIO_Init();
}

int8_t bsp_init(void)
{
    peripheral_init();

    bsp_led_init(DEBUG_LED);
    bsp_uart_init(DEBUG_UART);

    return 0;
}

int8_t bsp_deinit(void)
{
    bsp_led_deinit(DEBUG_LED);
    bsp_uart_deinit(DEBUG_UART);
    return 0;
}
