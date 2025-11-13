#include "scene_scoreboard.h"

#include "draw_string.h"

#include <libdragon.h>
#include <string.h>

static SceneScoreboardState scene_state;

#define MENU_HEADER "HIGHSCORE"

void scene_init_scoreboard(SceneManager *scene_manager)
{
  ecs_init(&scene_state.ecs);
  render_system_init(&scene_state.renderSystem);

  surface_t *disp = display_get();
  graphics_fill_screen(disp, 0x0);
  graphics_set_color(0xFFFFFFFF, 0x0);

  print_centered_text(disp, 10, MENU_HEADER);

  display_show(disp);
}

void scene_update_scoreboard(SceneManager *scene_manager, GameCommand command)
{
  if (command == COMMAND_SELECT)
    scene_manager_set_scene(scene_manager, SCENE_MAIN_MENU);
}

void scene_exit_scoreboard()
{
  surface_t *disp = display_get();
  graphics_fill_screen(disp, 0x0);
  display_show(disp);
  memset(&scene_state, 0, sizeof(SceneScoreboardState));
}