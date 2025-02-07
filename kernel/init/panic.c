
#include <kernel.h>
#include <kernel/vga_textmode/vga_textmode.h>

/* Prints a message to the console, then infinitely spins */
void panic(const char *msg, const char *file, uint32_t line) {
  asm volatile("cli");
  vga_printf("PANIC: %s at %s:%d\n", msg, file, line);
  for (;;)
    ;
}

/* Prints an assertion message to the console, then infinitely spins */
void panic_assert(const char *file, uint32_t line, const char *desc) {
  asm volatile("cli");
  vga_printf("ASSERT-PANIC: %s at %s:%d\n", desc, file, line);
  for (;;)
    ;
}