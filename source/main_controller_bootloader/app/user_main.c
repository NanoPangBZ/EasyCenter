#include "bsp.h"
#include "component.h"
#include "qspi_flash_test.h"

#define TAG     "user_main"

void user_main(void)
{
    bsp_init();
    component_init();

    LOG_DEBUG(TAG,"start up.");

    while(1)
    {
        bsp_delay_ms(500);
        bsp_led_set(0,1);
        bsp_delay_ms(500);
        bsp_led_set(0,0);
    }
}


