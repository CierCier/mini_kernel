
#pragma once

#include <kernel.h>
#include <kernel/init/interrupt.h>
#include <stddef.h>
#include <stdint.h>

struct _idt_entry {
  uint16_t low_off; /* offset bits 15..0 */
  uint16_t sel;     /* selector */
  uint8_t zero0;    /* zeroed out */
  uint8_t flags;    /* flag information */
  uint16_t mid_off; /* offset bits 31..16 */
  uint32_t hi_off;  /* offset bits 63..32 */
  uint32_t zero1;   /* zeroed out */
} __attribute__((packed));

struct _idt {
  uint16_t limit; /* length of the table */
  uint64_t base;  /* base address */
} __attribute__((packed));

/* Initializes the idt */
void idt_init();
