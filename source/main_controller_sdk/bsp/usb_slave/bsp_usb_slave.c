#include "bsp_usb_slave.h"
#include "usb_device.h"
#include "usbd_customhid.h"

int32_t bsp_usb_slave_init(void)
{
    MX_USB_DEVICE_Init();
    return 0;
}

int32_t bsp_usb_write(uint8_t* buf , uint16_t len)
{
    return 0;
}

void bsp_usb_recieve_callback_register(usb_cb_t cb,void* ctx)
{
    return;
}

