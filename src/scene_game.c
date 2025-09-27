#include "scene_game.h"
#include "systems.h"

#include <libdragon.h>
#include <string.h>

static SceneGameState scene_state;

void scene_init_game(SceneManager *scene_manager)
{
  ecs_init(&scene_state.ecs);

  Entity player = ecs_create_entity(&scene_state.ecs);
  ecs_add_position(&scene_state.ecs, player, (Position){.x = 100.0f, .y = 100.0f});
  ecs_add_velocity(&scene_state.ecs, player, (Velocity){.vx = 0.0f, .vy = 0.0f});

  surface_t *display = display_get();
  uint32_t color = graphics_make_color(0xCC, 0xCC, 0xCC, 0xFF);
  graphics_fill_screen(display, color);
  display_show(display);
}

void scene_update_game(SceneManager *scene_manager, GameCommand command)
{
  surface_t *display = display_get();

  uint32_t bg_color = graphics_make_color(0xCC, 0xCC, 0xCC, 0xFF);
  graphics_fill_screen(display, bg_color);

  int player = 0; // Assuming player entity is 0

  graphics_draw_text(display, 10, 10, "Press R to accelerate and L to slow down");

  static GameCommand last_command = COMMAND_NONE;

  if (command == COMMAND_ACCELERATE)
  {
    Velocity *vel = &scene_state.ecs.velocities[player];

    vel->vx += 0.1f;
    vel->vx = fmaxf(0.0f, fminf(vel->vx, 5.0f));

    last_command = COMMAND_ACCELERATE; 
  }

  if (command == COMMAND_SLOWDOWN)
  {
    Velocity *vel = &scene_state.ecs.velocities[player];

    vel->vx -= 0.1f;
    vel->vx = fmaxf(0.0f, fminf(vel->vx, 5.0f));

    last_command = COMMAND_SLOWDOWN;
  }

  if (scene_state.ecs.active_entities[player] && scene_state.ecs.has_position[player])
  {
    system_movement(&scene_state.ecs);

    Position pos = scene_state.ecs.positions[player];
    Velocity vel = scene_state.ecs.velocities[player];

    char buffer[64];
    snprintf(buffer, sizeof(buffer), "Player X: %.3f", pos.x);
    graphics_draw_text(display, 10, 30, buffer);

    snprintf(buffer, sizeof(buffer), "Player Y: %.3f", pos.y);
    graphics_draw_text(display, 10, 50, buffer);

    snprintf(buffer, sizeof(buffer), "Player VX: %.3f", vel.vx);
    graphics_draw_text(display, 10, 70, buffer);

    snprintf(buffer, sizeof(buffer), "Player VY: %.3f", vel.vy);
    graphics_draw_text(display, 10, 90, buffer);
  }

  char command_buffer[64];
  switch (last_command)
  {
  case COMMAND_ACCELERATE:
    snprintf(command_buffer, sizeof(command_buffer), "Last Command: Accelerate");
    break;
  case COMMAND_SLOWDOWN:
    snprintf(command_buffer, sizeof(command_buffer), "Last Command: Slowdown");
    break;
  case COMMAND_SELECT:
    snprintf(command_buffer, sizeof(command_buffer), "Last Command: Select");
    break;
  default:
    snprintf(command_buffer, sizeof(command_buffer), "Last Command: None");
    break;
  }
  graphics_draw_text(display, 10, 110, command_buffer);

  display_show(display);

  if (command == COMMAND_SELECT)
    scene_manager_set_scene(scene_manager, SCENE_SCOREBOARD);
}

void scene_exit_game()
{
  memset(&scene_state, 0, sizeof(SceneGameState));
}