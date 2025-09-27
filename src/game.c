#include "game.h"

#include "scenes.h"
#include "input.h"
#include "commandmapping.h"

#include <stdio.h>
#include <libdragon.h>

#define TICK_TIME_MS (1000 / TICKS_PER_SECOND)

void game_loop(GameContext *game)
{
  uint32_t last_tick = timer_ticks();
  uint32_t lag = 0;

  while (1)
  {
    uint32_t now = timer_ticks();
    uint32_t elapsed = TICKS_DISTANCE(now, last_tick);

    lag += elapsed;
    last_tick = now;

    // Run one or more updates to catch up if behind
    while (lag >= TICK_TIME_MS)
    {
      input_read(&game->input_state);
      GameCommand command = scene_input_to_command_mapping(game->scene_manager.current_scene, &game->input_state);
      scene_manager_update(&game->scene_manager, command);

      lag -= TICK_TIME_MS;
    }
  }
}
