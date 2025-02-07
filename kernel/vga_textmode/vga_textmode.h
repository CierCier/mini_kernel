#pragma once

#include <stdint.h>

enum {
  BLACK = 0,
  BLUE = 1,
  GREEN,
  CYAN,
  RED,
  MAGENTA,
  BROWN,
  LIGHT_GREY,
  DARK_GREY,
  LIGHT_BLUE,
  LIGHT_GREEN,
  LIGHT_CYAN,
  LIGHT_RED,
  LIGHT_MAGENTA,
  YELLOW,
  WHITE
};

void vga_clear_screen(void);

void vga_putc(char c);
int vga_puts(char *str);

void vga_newline(void);
void vga_clearrow(uint16_t row);

int vga_printf(char *fmt, ...);

void set_cursor_pos(int x, int y);


void set_text_color(uint8_t fg, uint8_t bg);
void set_background_color(uint8_t bg);
