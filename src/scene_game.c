#include "scene_game.h"

#include "rendersystem.h"
#include "ecs.h"
#include "assets.h"

#include <libdragon.h>
#include <string.h>
#include <math.h>

static SceneGameState scene_state;

static const Rotation INITIAL_TABLE_ROTATION = (Rotation){0.0f, 0.0f, 0.0f};
static const float TABLE_ROTATION_STEP = 0.1f;

// Render system configuration
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
  ecs_add_rotation(&scene_state.ecs, player, INITIAL_TABLE_ROTATION);
}

float clampSingleRotation(float currentRotation, float initialRotation, float maxAngle)
{
  float deltaRotation = currentRotation - initialRotation;

  if (deltaRotation > maxAngle)
    return initialRotation + maxAngle;
  else if (deltaRotation < -maxAngle)
    return initialRotation - maxAngle;
  else
    return currentRotation;
}

Rotation clampTableRotation(Rotation rotation, float maxAngle)
{
  rotation.x = clampSingleRotation(rotation.x, INITIAL_TABLE_ROTATION.x, maxAngle);
  rotation.y = clampSingleRotation(rotation.y, INITIAL_TABLE_ROTATION.y, maxAngle);
  rotation.z = clampSingleRotation(rotation.z, INITIAL_TABLE_ROTATION.z, maxAngle);

  return rotation;
}

Rotation RotateTable(GameCommand command, Rotation currentRotation, float maxAngle)
{
  Rotation newRotation = currentRotation;

  switch (command)
  {
  case COMMAND_UP:
    newRotation.x += TABLE_ROTATION_STEP;
    break;

  case COMMAND_DOWN:
    newRotation.x -= TABLE_ROTATION_STEP;
    break;

  case COMMAND_LEFT:
    newRotation.y += TABLE_ROTATION_STEP;
    break;

  case COMMAND_RIGHT:
    newRotation.y -= TABLE_ROTATION_STEP;
    break;

  case COMMAND_UP_LEFT:
    newRotation.x += TABLE_ROTATION_STEP;
    newRotation.y += TABLE_ROTATION_STEP;
    break;

  case COMMAND_UP_RIGHT:
    newRotation.x += TABLE_ROTATION_STEP;
    newRotation.y -= TABLE_ROTATION_STEP;
    break;

  case COMMAND_DOWN_LEFT:
    newRotation.x -= TABLE_ROTATION_STEP;
    newRotation.y += TABLE_ROTATION_STEP;
    break;

  case COMMAND_DOWN_RIGHT:
    newRotation.x -= TABLE_ROTATION_STEP;
    newRotation.y -= TABLE_ROTATION_STEP;
    break;

  default:
    break;
  }

  newRotation = clampTableRotation(newRotation, maxAngle);

  return newRotation;
}

void scene_update_game(SceneManager *scene_manager, GameCommand command)
{
  Rotation currentRot = ecs_get_rotation(&scene_state.ecs, 0);

  if (command == COMMAND_SELECT)
  {
    scene_manager_set_scene(scene_manager, SCENE_SCOREBOARD);
  }

  Rotation newRot = RotateTable(command, currentRot, M_PI / 4);
  ecs_add_rotation(&scene_state.ecs, 0, newRot);
  render_system_update_frame(&scene_state.renderSystem, &scene_state.ecs);
}

void scene_exit_game()
{
  render_system_cleanup(&scene_state.renderSystem);
}
