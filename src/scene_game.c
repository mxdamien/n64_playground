#include "scene_game.h"
#include "rendersystem.h"
#include "ecs.h"
#include <libdragon.h>
#include <string.h>

static SceneGameState scene_state;

void scene_init_game(SceneManager *scene_manager)
{
  ecs_init(&scene_state.ecs);
  render_system_init(&scene_state.renderSystem);

  render_system_register_callbacks(&scene_state.ecs, &scene_state.renderSystem);

  Entity player = ecs_create_entity(&scene_state.ecs);
  ecs_add_position(&scene_state.ecs, player, (Position){0.0f, 0.0f, 0.0f});
  ecs_add_velocity(&scene_state.ecs, player, (Velocity){0.0f, 0.0f, 0.0f});

  render_system_register_entity(&scene_state.renderSystem, player, 1); // Texture ID "1" for now
}

void scene_update_game(SceneManager *scene_manager, GameCommand command)
{
  render_system_update(&scene_state.renderSystem);

  if (command == COMMAND_SELECT)
    scene_manager_set_scene(scene_manager, SCENE_SCOREBOARD);

  if (command == COMMAND_UP)
  {
    Position pos = ecs_get_position(&scene_state.ecs, 0);
    ecs_add_position(&scene_state.ecs, 0, (Position){pos.x, pos.y - 1.2, pos.z});
  }

  if (command == COMMAND_DOWN)
  {
    Position pos = ecs_get_position(&scene_state.ecs, 0);
    ecs_add_position(&scene_state.ecs, 0, (Position){pos.x, pos.y + 1.2, pos.z});
  }

  if (command == COMMAND_LEFT)
  {
    Position pos = ecs_get_position(&scene_state.ecs, 0);
    ecs_add_position(&scene_state.ecs, 0, (Position){pos.x - 1.2, pos.y, pos.z});
  }

  if (command == COMMAND_RIGHT)
  {
    Position pos = ecs_get_position(&scene_state.ecs, 0);
    ecs_add_position(&scene_state.ecs, 0, (Position){pos.x + 1.2, pos.y, pos.z});
  }
}

void scene_exit_game()
{
  memset(&scene_state, 0, sizeof(SceneGameState));
}
