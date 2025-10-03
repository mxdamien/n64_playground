#include "rendersystem.h"
#include "ecs.h"

#include <string.h>
#include <libdragon.h>

void render_system_init(RenderSystem *const render_system)
{
  memset(render_system, 0, sizeof(RenderSystem));
}

void render_system_register_entity(RenderSystem *const render_system, Entity e, uint32_t texture_id)
{
  if (render_system->entity_count < MAX_RENDER_ENTITIES)
  {
    render_system->entities[render_system->entity_count] = e;
    render_system->texture_ids[render_system->entity_count] = texture_id;
    render_system->entity_count++;
  }
}

void on_position_updated(Position pos, Entity e, void *context)
{
  if (context == NULL)
    return;

  // RenderSystem *render_system = (RenderSystem *)context;

  char buffer[64];

  surface_t *display = display_get();

  graphics_fill_screen(display, graphics_make_color(0, 0, 0, 255));

  snprintf(buffer, sizeof(buffer), "on_position_updated: Player X: %.3f", pos.x);
  graphics_draw_text(display, 0, 0, buffer);

  snprintf(buffer, sizeof(buffer), "on_position_updated: Player Y: %.3f", pos.y);
  graphics_draw_text(display, 0, 10, buffer);

  uint32_t red_color = graphics_make_color(255, 0, 0, 0); // Red color

  int box_size = 20;

  int x = (int)pos.x - (box_size / 2); // Center the box around pos.x
  int y = (int)pos.y - (box_size / 2); // Center the box around pos.y

  graphics_draw_box(display, x, y, box_size, box_size, red_color);
  display_show(display);
}

void render_system_register_callbacks(ECS *const ecs, RenderSystem *const render_system)
{
  ecs_register_position_callback(ecs, on_position_updated, render_system);
}

void render_system_update(const RenderSystem *const render_system)
{
}

void render_entity(Entity e, vec3 position, uint32_t texture_id)
{
}
