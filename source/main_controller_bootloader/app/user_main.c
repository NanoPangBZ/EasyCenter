#include "main.h"
#include <string.h>

#include "bsp.h"
#include "component.h"

#define TAG     "user_main"

void user_main(void)
{
    bsp_init();
    component_init();

    LOG_DEBUG(TAG,"This is a debug log");
    LOG_INFO(TAG,"This is a info log");
    LOG_WARN(TAG,"This is a warn log");
    LOG_ERR(TAG,"This is a err log");

    while(1)
    {
        HAL_Delay(1000);
        bsp_led_set(0,1);
        LOG_INFO(TAG,"led on.");
        HAL_Delay(1000);
        bsp_led_set(0,0);
        LOG_INFO(TAG,"led off");
    }
}


