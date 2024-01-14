#pragma once

#include <stdint.h>
#include "./w25qxx/w25qxx.h"
#include "./led/bsp_led.h"

int8_t bsp_init(void);
int8_t bsp_deinit(void);
