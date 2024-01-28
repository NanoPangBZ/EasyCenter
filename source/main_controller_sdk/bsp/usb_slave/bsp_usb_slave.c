#include "bsp_usb_slave.h"
#include "usb_device.h"
#include "usbd_customhid.h"

int32_t bsp_usb_slave_init(void)
{
    MX_USB_DEVICE_Init();
    return 0;
}
