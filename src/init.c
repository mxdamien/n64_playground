#include "init.h"

#include <libdragon.h>

void init_system(void)
{
  init_display();
  console_init();
}

void init_display()
{
  display_init(RESOLUTION_320x240, DEPTH_32_BPP, 2, GAMMA_NONE, FILTERS_RESAMPLE);
  display_context_t disp = display_get();
  display_show(disp);
}