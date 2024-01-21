#pragma once

#include <stdint.h>
#include "./led/bsp_led.h"
#include "./uart/bsp_uart.h"
#include "./qspi_flash/bsp_qspi_flash.h"

int8_t bsp_init(void);
int8_t bsp_deinit(void);
