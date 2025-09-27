#include "scene_main_menu.h"

#include <libdragon.h>
#include <string.h>

static SceneMainMenuState scene_state;

void scene_init_main_menu(SceneManager *scene_manager)
{
  ecs_init(&scene_state.ecs);

  surface_t *display = display_get();
  uint32_t color = graphics_make_color(0xCC, 0xCC, 0xFF, 0xFF);
  graphics_fill_screen(display, color);
  display_show(display);
}

void scene_update_main_menu(SceneManager *scene_manager, GameCommand command)
{
  if (command == COMMAND_SELECT)
    scene_manager_set_scene(scene_manager, SCENE_GAME);
}

void scene_exit_main_menu()
{
  memset(&scene_state, 0, sizeof(SceneMainMenuState));
}