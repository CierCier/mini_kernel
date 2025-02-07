#pragma once
#include <stddef.h>
#include <stdint.h>

// define GDT structure for 64-bit mode
struct gdt_entry {
  uint16_t limit_low;
  uint16_t base_low;
  uint8_t base_middle;
  uint8_t access;
  uint8_t granularity;
  uint8_t base_high;
} __attribute__((packed));
