#include "commandmapping.h"

GameCommand scene_input_to_command_mapping(scene_t scene, const InputState *input_state)
{
  switch (scene)
  {
  case SCENE_MAIN_MENU:
    if (input_state->controller[0].buttons.A.pressed)
    {
      return COMMAND_SELECT;
    }
    return COMMAND_NONE;

  case SCENE_GAME:
    if (input_state->controller[0].buttons.R.pressed)
    {
      return COMMAND_ACCELERATE;
    }
    if (input_state->controller[0].buttons.L.pressed)
    {
      return COMMAND_SLOWDOWN;
    }
    return COMMAND_NONE;

  case SCENE_SCOREBOARD:
    if (input_state->controller[0].buttons.A.pressed)
    {
      return COMMAND_SELECT;
    }
    return COMMAND_NONE;

  default:
    return COMMAND_NONE;
  }
}
