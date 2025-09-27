#include "scene_scoreboard.h"

#include <libdragon.h>
#include <string.h>

static SceneScoreboardState scene_state;

void scene_init_scoreboard(SceneManager *scene_manager)
{
  ecs_init(&scene_state.ecs);

  surface_t *display = display_get();
  uint32_t color = graphics_make_color(0xCC, 0x33, 0xFF, 0xFF);
  graphics_fill_screen(display, color);
  display_show(display);
}

void scene_update_scoreboard(SceneManager *scene_manager, GameCommand command)
{
  if (command == COMMAND_SELECT)
    scene_manager_set_scene(scene_manager, SCENE_MAIN_MENU);
}

void scene_exit_scoreboard()
{
  memset(&scene_state, 0, sizeof(SceneScoreboardState));
}