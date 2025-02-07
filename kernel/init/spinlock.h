#pragma once
#include <stdint.h>

extern void spin_lock(uint32_t *lock);
extern void spin_unlock(uint32_t *lock);