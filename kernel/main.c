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
uint32_t pit_freq = 100; // 100 Hz

void check_locks() {
  vga_printf("Testing SpinLocks\n");
  vga_printf("Attempting to lock: %x \n", &lock, &lock);
  spin_lock(&lock);
  vga_printf("Lock acquired: %x\n", &lock);
  spin_unlock(&lock);
  vga_printf("Lock Released: %x\n", &lock);
}

void check_printer() {
  vga_printf("Testing VGA Printer\n");
  vga_printf("Printing a string: %s\n", "Hello World");
  vga_printf("Printing a char: %c\n", 'A');
  vga_printf("Printing a decimal: %d\n", 1234);
  vga_printf("Printing a hex: %x\n", 1234);
  vga_printf("Printing a pointer: %p\n", &lock);
}

void kmain(void) {

  vga_clear_screen();
  check_printer();

  vga_printf("Initializing GDT\n");
  vga_printf("Initializing IDT\n");
  idt_init();
  check_locks();

  pit_init(pit_freq);

  check_cpuid_features();

  vga_printf("Initializing Keyboard\n");
  kbd_drv_init();

  uint8_t ch = 0;

  while (1) {
    if ((ch = kbd_get_key()) != 0) {
      vga_printf("Got Key Press: %c\n", ch);
    }
  }

  while (1) {

    asm volatile("hlt");
  }
}

void InitGDT(void) {}

const char *ASCII_ART = R"(
  
)";
