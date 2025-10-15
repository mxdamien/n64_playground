#ifndef DRAW_STRING_H
#define DRAW_STRING_H

#include <string.h>
#include <libdragon.h>

inline int get_string_width(const char *str)
{
  return strlen(str) * 8; // Assuming 8 pixels per character
}

inline void print_centered_text(surface_t *disp, int y, const char *str)
{
  int screen_width = disp->width;
  int text_width = get_string_width(str);
  int x_position = (screen_width - text_width) / 2; // Centered x position
  graphics_draw_text(disp, x_position, y, str);
}

#endif