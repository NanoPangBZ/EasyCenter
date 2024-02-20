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
        bsp_led_set(DEBUG_LED,1);
        bsp_delay_ms(100);
        bsp_led_set(DEBUG_LED,0);
    }
}


