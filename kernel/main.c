#include <kernel.h>

#include <kernel/boot/gdt.h>
#include <kernel/device/keyboard.h>
#include <kernel/init/cpuid.h>
#include <kernel/init/idt.h>
#include <kernel/init/interrupt.h>
#include <kernel/init/io.h>
#include <kernel/init/pit.h>
#include <kernel/init/spinlock.h>
#include <kernel/vga_textmode/vga_textmode.h>
#include <stdint.h>

uint32_t lock = 0;

void check_locks() {
  vga_printf("Testing SpinLocks\n");
  vga_printf("Attempting to lock: %x = %u\n", &lock, lock);
  spin_lock(&lock);
  vga_printf("Lock acquired: %x =  %u", &lock, lock);
  spin_lock(&lock);
  vga_printf("Lock Released: %x", &lock);
}

void kmain(void) {

  vga_printf("Initializing IDT\n");
  vga_printf("Hello world\n");
  idt_init();

  kbd_drv_init();

  uint8_t ch = 0;

  while (1) {
    if ((ch = kbd_get_key()) != 0) {
      vga_printf("Got Key %c\n", ch);
    }
  }

  while (1) {

    asm volatile("hlt");
  }
}

void InitGDT(void) {}