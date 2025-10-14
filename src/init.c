#include "init.h"

#include <libdragon.h>
#include <t3d/t3d.h>

void init_display();

void init_system()
{
  init_display();
  rdpq_init();
  t3d_init((T3DInitParams){});
  console_init();
}

void init_display()
{
  display_init(RESOLUTION_320x240, DEPTH_32_BPP, 2, GAMMA_NONE, FILTERS_RESAMPLE);
  display_context_t disp = display_get();
  display_show(disp);
}