#include "scene_scoreboard.h"

#include <libdragon.h>
#include <string.h>

static SceneScoreboardState scene_state;

void scene_init_scoreboard(SceneManager *scene_manager)
{
  ecs_init(&scene_state.ecs);
  render_system_init(&scene_state.renderSystem);

  render_system_register_callbacks(&scene_state.ecs, &scene_state.renderSystem);
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