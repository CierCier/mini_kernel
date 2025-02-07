#pragma once

#ifdef __linux__
#error "Not supported on Linux" "Are you sure you're using the right cross-compiler?"
#endif

#include <kernel/init/spinlock.h>
#include <stdint.h>

#define PANIC(msg) panic(msg, __FILE__, __LINE__)
#define ASSERT(b) ((b) ? (void)0 : panic_assert(__FILE__, __LINE__, #b))

#define min(a, b) ((a > b) ? b : a)
#define max(a, b) ((a > b) ? a : b)
#define abs(a) ((a >= 0) ? a : -a)

/* Print a kernel message */
void printf(const char *fmt, ...);

/* Panic prototypes */
extern void panic(const char *msg, const char *file, uint32_t line);
extern void panic_assert(const char *file, uint32_t line, const char *desc);

extern uint32_t rand();
