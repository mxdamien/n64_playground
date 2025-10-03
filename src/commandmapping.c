#include "commandmapping.h"

GameCommand scene_input_to_command_mapping(scene_t scene, const InputState *const input_state)
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
    if (input_state->controller[0].buttons.A.pressed)
    {
      return COMMAND_SELECT;
    }
    if (input_state->controller[0].buttons.Up.pressed || input_state->controller[0].buttons.Up.held)
    {
      return COMMAND_UP;
    }
    if (input_state->controller[0].buttons.Down.pressed || input_state->controller[0].buttons.Down.held)
    {
      return COMMAND_DOWN;
    }
    if (input_state->controller[0].buttons.Left.pressed || input_state->controller[0].buttons.Left.held)
    {
      return COMMAND_LEFT;
    }
    if (input_state->controller[0].buttons.Right.pressed || input_state->controller[0].buttons.Right.held)
    {
      return COMMAND_RIGHT;
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
