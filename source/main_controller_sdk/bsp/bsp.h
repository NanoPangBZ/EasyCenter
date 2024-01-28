#pragma once

#include <stdio.h>
#include <stdint.h>
#include "./led/bsp_led.h"
#include "./uart/bsp_uart.h"
#include "./qspi_flash/bsp_qspi_flash.h"
#include "./usb_slave/bsp_usb_slave.h"

int8_t bsp_init(void);
int8_t bsp_deinit(void);
void bsp_delay_ms(uint32_t ms);
