#pragma once
#include <stdint.h>

void kbd_drv_init();
void kbd_drv_term();
uint8_t kbd_get_key();