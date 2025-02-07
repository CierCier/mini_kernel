
#include "klibc/kstring.h"
#include <kernel/vga_textmode/vga_textmode.h>

#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>

struct VGA_BUFFER_ENTRY {
  uint8_t character;
  uint8_t color;
};

#define VGA_WIDTH 80
#define VGA_HEIGHT 25
#define VGA_BUFFER_SIZE (VGA_WIDTH * VGA_HEIGHT)

static uint8_t VGA_COLOR = 0x0F;

struct VGA_WRITE_CURSOR {
  uint16_t x;
  uint16_t y;
} cursor = {0, 0};

struct VGA_BUFFER_ENTRY *const VGA_BUFFER = (struct VGA_BUFFER_ENTRY *)0xB8000;

void vga_clear_screen(void) {
  for (uint16_t y = 0; y < VGA_HEIGHT; y++) {
    for (uint16_t x = 0; x < VGA_WIDTH; x++) {
      VGA_BUFFER[y * VGA_WIDTH + x].character = ' ';
      VGA_BUFFER[y * VGA_WIDTH + x].color = 0;
    }
  }
}

void vga_putc(char c) {
  switch (c) {
  case '\n':
    vga_newline();
    return;
  case '\t':
    for (int i = 0; i < 4; i++) {
      vga_putc(' ');
    }
    break;

  default:
    VGA_BUFFER[cursor.y * VGA_WIDTH + cursor.x].character = c;
    VGA_BUFFER[cursor.y * VGA_WIDTH + cursor.x].color = VGA_COLOR;
    cursor.x++;
  }

  if (cursor.x >= VGA_WIDTH) {
    vga_newline();
  }
}

int vga_puts(char *str) {
  int i = 0;
  while (str[i] != '\0') {
    vga_putc(str[i++]);
  }

  return i;
}

void vga_newline(void) {
  uint16_t y = cursor.y;

  if (y < VGA_HEIGHT - 1) {
    cursor.y++;
  } else {

    for (y = 0; y < VGA_HEIGHT - 1; y++) {
      for (uint16_t x = 0; x < VGA_WIDTH; x++) {
        VGA_BUFFER[y * VGA_WIDTH + x] = VGA_BUFFER[(y + 1) * VGA_WIDTH + x];
      }
    }
    vga_clearrow(VGA_HEIGHT - 1);
  }
  cursor.x = 0;
}

void vga_clearrow(uint16_t row) {
  for (uint16_t x = 0; x < VGA_WIDTH; x++) {
    VGA_BUFFER[row * VGA_WIDTH + x].character = ' ';
    VGA_BUFFER[row * VGA_WIDTH + x].color = 0;
  }
}

int vga_printf(char *format, ...) {
  char buffer[256]; // Internal buffer
  size_t bi = 0;    // Buffer index
  size_t printed = 0;
  va_list args;

  va_start(args, format);

  for (size_t i = 0; format[i] != '\0'; i++) {
    if (bi >= sizeof(buffer) - 1) { // Ensure buffer doesn’t overflow
      buffer[bi] = '\0';
      printed += vga_puts(buffer);
      bi = 0;
    }

    if (format[i] == '%') {
      i++;
      if (format[i] == '\0')
        break; // Prevent out-of-bounds access

      switch (format[i]) {
      case 'd': { // Signed integer
        int num = va_arg(args, int);
        char num_buf[12]; // Enough for 32-bit int (-2147483648)
        itoa(num, num_buf, 10);
        size_t len = strlen(num_buf);
        if (bi + len < sizeof(buffer) - 1) {
          strcpy(buffer + bi, num_buf);
          bi += len;
        }
        break;
      }
      case 'u': { // Unsigned integer
        unsigned int num = va_arg(args, unsigned int);
        char num_buf[11];
        utoa(num, num_buf, 10);
        size_t len = strlen(num_buf);
        if (bi + len < sizeof(buffer) - 1) {
          strcpy(buffer + bi, num_buf);
          bi += len;
        }
        break;
      }
      case 'x':
      case 'X': { // Hexadecimal
        unsigned int num = va_arg(args, unsigned int);
        char num_buf[10];
        utoa(num, num_buf, 16);
        size_t len = strlen(num_buf);
        if (bi + len < sizeof(buffer) - 1) {
          strcpy(buffer + bi, num_buf);
          bi += len;
        }
        break;
      }
      case 'o': { // Octal
        unsigned int num = va_arg(args, unsigned int);
        char num_buf[12];
        utoa(num, num_buf, 8);
        size_t len = strlen(num_buf);
        if (bi + len < sizeof(buffer) - 1) {
          strcpy(buffer + bi, num_buf);
          bi += len;
        }
        break;
      }
      case 'p': { // Pointer address
        void *ptr = va_arg(args, void *);
        char num_buf[18] = "0x"; // Prefix with 0x
        utoa((uintptr_t)ptr, num_buf + 2, 16);
        size_t len = strlen(num_buf);
        if (bi + len < sizeof(buffer) - 1) {
          strcpy(buffer + bi, num_buf);
          bi += len;
        }
        break;
      }
      case 'l': { // Long/Long Long variants
        i++;
        if (format[i] == 'l') { // Long long
          i++;
          switch (format[i]) {
          case 'd': {
            long long num = va_arg(args, long long);
            char num_buf[21]; // Large enough for 64-bit int
            lltoa(num, num_buf, 10);
            size_t len = strlen(num_buf);
            if (bi + len < sizeof(buffer) - 1) {
              strcpy(buffer + bi, num_buf);
              bi += len;
            }
            break;
          }
          case 'u': {
            unsigned long long num = va_arg(args, unsigned long long);
            char num_buf[21];
            ulltoa(num, num_buf, 10);
            size_t len = strlen(num_buf);
            if (bi + len < sizeof(buffer) - 1) {
              strcpy(buffer + bi, num_buf);
              bi += len;
            }
            break;
          }
          case 'x':
          case 'X': {
            unsigned long long num = va_arg(args, unsigned long long);
            char num_buf[21];
            ulltoa(num, num_buf, 16);
            size_t len = strlen(num_buf);
            if (bi + len < sizeof(buffer) - 1) {
              strcpy(buffer + bi, num_buf);
              bi += len;
            }
            break;
          }
          default:
            if (bi < sizeof(buffer) - 3) {
              buffer[bi++] = '%';
              buffer[bi++] = 'l';
              buffer[bi++] = format[i];
            }
            break;
          }
        } else { // Single long type
          switch (format[i]) {
          case 'd': {
            long num = va_arg(args, long);
            char num_buf[21];
            lltoa(num, num_buf, 10);
            size_t len = strlen(num_buf);
            if (bi + len < sizeof(buffer) - 1) {
              strcpy(buffer + bi, num_buf);
              bi += len;
            }
            break;
          }
          case 'u': {
            unsigned long num = va_arg(args, unsigned long);
            char num_buf[21];
            ulltoa(num, num_buf, 10);
            size_t len = strlen(num_buf);
            if (bi + len < sizeof(buffer) - 1) {
              strcpy(buffer + bi, num_buf);
              bi += len;
            }
            break;
          }
          default:
            if (bi < sizeof(buffer) - 2) {
              buffer[bi++] = '%';
              buffer[bi++] = 'l';
              buffer[bi++] = format[i];
            }
            break;
          }
        }
        break;
      }
      case 'f': { // Floating-point number
        double num = va_arg(args, double);
        char num_buf[32];
        ftoa(num, num_buf, 6); // 6 decimal places
        size_t len = strlen(num_buf);
        if (bi + len < sizeof(buffer) - 1) {
          strcpy(buffer + bi, num_buf);
          bi += len;
        }
        break;
      }
      case 's': { // String
        const char *str = va_arg(args, const char *);
        size_t len = strlen(str);
        if (bi + len < sizeof(buffer) - 1) {
          strcpy(buffer + bi, str);
          bi += len;
        }
        break;
      }
      case 'c': { // Character
        char c = (char)va_arg(args, int);
        if (bi < sizeof(buffer) - 1) {
          buffer[bi++] = c;
        }
        break;
      }
      case '%': { // Literal '%'
        if (bi < sizeof(buffer) - 1) {
          buffer[bi++] = '%';
        }
        break;
      }
      default:
        if (bi < sizeof(buffer) - 2) {
          buffer[bi++] = '%';
          buffer[bi++] = format[i];
        }
        break;
      }
    } else {
      if (bi < sizeof(buffer) - 1) {
        buffer[bi++] = format[i];
      }
    }
  }

  va_end(args);

  buffer[bi] = '\0'; // Ensure string termination
  return printed + vga_puts(buffer);
}

void set_cursor_pos(int x, int y);

void set_text_color(uint8_t fg, uint8_t bg);
void set_background_color(uint8_t bg);
