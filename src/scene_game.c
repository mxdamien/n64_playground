#include "scene_game.h"

#include "rendersystem.h"
#include "ecs.h"
#include "assets.h"

#include <libdragon.h>
#include <string.h>

static SceneGameState scene_state;

void render_run();

void scene_init_game(SceneManager *scene_manager)
{
  ecs_init(&scene_state.ecs);
  render_system_init(&scene_state.renderSystem);

  Entity player = ecs_create_entity(&scene_state.ecs);
  ecs_add_position(&scene_state.ecs, player, (Position){0.0f, 0.0f, 0.0f});
  ecs_add_velocity(&scene_state.ecs, player, (Velocity){0.0f, 0.0f, 0.0f});
  ecs_add_model(&scene_state.ecs, player, ID_MODEL_TABLE);
}

void scene_update_game(SceneManager *scene_manager, GameCommand command)
{
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

  render_run();
}

void render_run()
{
  render_system_begin_frame(&scene_state.renderSystem);
  render_entities(&scene_state.renderSystem, &scene_state.ecs);
  render_system_end_frame(&scene_state.renderSystem);
}

void scene_exit_game()
{
}
