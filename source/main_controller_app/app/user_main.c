#include "main.h"
#include "gpio.h"
#include "bsp.h"

void user_main(void)
{
    bsp_init();

    while(1)
    {
        HAL_Delay(1000);
        bsp_led_set(0,1);
        HAL_Delay(1000);
        bsp_led_set(0,0);
    }
}


