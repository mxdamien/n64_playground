#include "scene_game.h"

#include "rendersystem.h"
#include "ecs.h"
#include "assets.h"

#include <libdragon.h>
#include <string.h>
#include <math.h>

static SceneGameState scene_state;

static const RenderSystemConfig render_config = {
    // Camera Configuration
    .camera_position = {{0, 10, 40}},
    .camera_target = {{0, 0, 0}},
    .camera_up = {{0, 1, 0}},
    .camera_fov = 85.0f,
    .camera_near_plane = 1.0f,
    .camera_far_plane = 200.0f,
    // Viewport Configuration
    .viewport_fov = 85.0f,
    .viewport_near_plane = 10.0f,
    .viewport_far_plane = 150.0f,
    // Lighting Configuration
    .ambient_color = {80, 80, 100, 0xFF},
    .directional_color = {0xEE, 0xAA, 0xAA, 0xFF},
    .light_direction = {{-1.0f, 1.0f, 1.0f}},
    // Scaling
    .entity_scale = 0.1f,
};

void scene_init_game(SceneManager *scene_manager)
{
  ecs_init(&scene_state.ecs);

  render_system_init(&scene_state.renderSystem, &render_config);

  Entity player = ecs_create_entity(&scene_state.ecs);
  ecs_add_position(&scene_state.ecs, player, (Position){0.0f, 0.0f, 0.0f});
  ecs_add_velocity(&scene_state.ecs, player, (Velocity){0.0f, 0.0f, 0.0f});
  ecs_add_model(&scene_state.ecs, player, ID_MODEL_TABLE);
  ecs_add_tilt(&scene_state.ecs, player, (Tilt){0.0f, 45.0f});
}

#include <math.h>

void scene_update_game(SceneManager *scene_manager, GameCommand command)
{
  Tilt *tilt = ecs_get_tilt(&scene_state.ecs, 0);
  if (tilt == NULL)
    return;

  Position pos = ecs_get_position(&scene_state.ecs, 0);

  const float TILT_SPEED = 5.0f;
  const float MAX_TILT_ANGLE = 45.0f;

  if (command == COMMAND_SELECT)
  {
    scene_manager_set_scene(scene_manager, SCENE_SCOREBOARD);
  }

  if (command == COMMAND_UP)
  {
    tilt->current_tilt_angle += TILT_SPEED;
    if (tilt->current_tilt_angle > MAX_TILT_ANGLE)
    {
      tilt->current_tilt_angle = MAX_TILT_ANGLE;
    }

    float tilt_radians = tilt->current_tilt_angle * M_PI / 180.0f;
    pos.x += sin(tilt_radians) * 1.2f;
    pos.z += cos(tilt_radians) * 1.2f;

    ecs_add_position(&scene_state.ecs, 0, pos);
  }

  if (command == COMMAND_DOWN)
  {
    tilt->current_tilt_angle -= TILT_SPEED;
    if (tilt->current_tilt_angle < -MAX_TILT_ANGLE)
    {
      tilt->current_tilt_angle = -MAX_TILT_ANGLE;
    }

    float tilt_radians = tilt->current_tilt_angle * M_PI / 180.0f;
    pos.x -= sin(tilt_radians) * 1.2f;
    pos.z -= cos(tilt_radians) * 1.2f;

    ecs_add_position(&scene_state.ecs, 0, pos);
  }

  if (command == COMMAND_LEFT)
  {
    pos.x -= 1.2f;
    ecs_add_position(&scene_state.ecs, 0, pos);
  }

  if (command == COMMAND_RIGHT)
  {
    pos.x += 1.2f;
    ecs_add_position(&scene_state.ecs, 0, pos);
  }

  render_system_update_frame(&scene_state.renderSystem, &scene_state.ecs);
}

void scene_exit_game()
{
  render_system_cleanup(&scene_state.renderSystem);
}
