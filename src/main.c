#include "game.h"
#include "init.h"
#include "input.h"

int main()
{
  init_system();

  GameContext game;
  scene_manager_init(&game.scene_manager);
  input_init(&game.input_state);

  game_loop(&game);

  return 0;
}
