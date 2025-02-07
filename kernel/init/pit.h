

#include <kernel/init/interrupt.h>
#include <stdint.h>

#define PIT_CLOCK_OSC 1193180

/* Initialize the timer to run at a frequency */
void pit_init(uint32_t freq);

uint32_t pit_get_countdown();
