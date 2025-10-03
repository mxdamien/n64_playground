#include "scene_main_menu.h"

#include <libdragon.h>
#include <string.h>

static SceneMainMenuState scene_state;

void scene_init_main_menu(SceneManager *scene_manager)
{
  ecs_init(&scene_state.ecs);
  render_system_init(&scene_state.renderSystem);

  render_system_register_callbacks(&scene_state.ecs, &scene_state.renderSystem);
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