#include "input.h"

#include "inputstate.h"

#include <stdio.h>
#include <libdragon.h>

void input_init(InputState *input_state)
{
  memset(input_state, 0, sizeof(InputState));
  joypad_init();
}

void input_read(InputState *input_state)
{
  joypad_poll();

  for (joypad_port_t port = 0; port < CONTROLLER_MAX; port++)
  {
    bool active = joypad_is_connected(port);
    set_joypad_active(&input_state->controller[port], active);

    if (!active)
      continue;

    bool rumble_supported = joypad_get_rumble_supported(port);
    set_rumble_supported(&input_state->controller[port], rumble_supported);

    bool rumble_active = joypad_get_rumble_active(port);
    set_rumble_active(&input_state->controller[port], rumble_active);

    bool memory_pack_available = joypad_get_memory_pack_available(port);
    set_memory_pack_available(&input_state->controller[port], memory_pack_available);

    joypad_style_t style = joypad_get_style(port);
    switch (style)
    {
    case JOYPAD_STYLE_N64:
      map_joypad_input_n64_standard_and_gamecube(&input_state->controller[port].buttons, port);
      break;
    case JOYPAD_STYLE_MOUSE:
      map_joypad_input_n64_mouse(&input_state->controller[port].buttons, port);
      break;
    case JOYPAD_STYLE_GCN:
      map_joypad_input_n64_standard_and_gamecube(&input_state->controller[port].buttons, port);
      break;
    default:
      break;
    }
  }
}

bool joypad_get_memory_pack_available(joypad_port_t port)
{
  return joypad_get_accessory_type(port) == JOYPAD_ACCESSORY_TYPE_CONTROLLER_PAK;
}

void map_joypad_input_n64_standard_and_gamecube(ButtonStates *state, joypad_port_t port)
{
  joypad_buttons_t btns_pressed = joypad_get_buttons_pressed(port);
  joypad_buttons_t btns_released = joypad_get_buttons_released(port);
  joypad_buttons_t btns_held = joypad_get_buttons_held(port);
  joypad_inputs_t inputs = joypad_get_inputs(port);

  // Digital buttons
  state->A.pressed = btns_pressed.a;
  state->A.released = btns_released.a;
  state->A.held = btns_held.a;

  state->B.pressed = btns_pressed.b;
  state->B.released = btns_released.b;
  state->B.held = btns_held.b;

  state->Start.pressed = btns_pressed.start;
  state->Start.released = btns_released.start;
  state->Start.held = btns_held.start;

  state->Z.pressed = btns_pressed.z;
  state->Z.released = btns_released.z;
  state->Z.held = btns_held.z;

  state->Left.pressed = btns_pressed.d_left;
  state->Left.released = btns_released.d_left;
  state->Left.held = btns_held.d_left;

  state->Right.pressed = btns_pressed.d_right;
  state->Right.released = btns_released.d_right;
  state->Right.held = btns_held.d_right;

  state->Up.pressed = btns_pressed.d_up;
  state->Up.released = btns_released.d_up;
  state->Up.held = btns_held.d_up;

  state->Down.pressed = btns_pressed.d_down;
  state->Down.released = btns_released.d_down;
  state->Down.held = btns_held.d_down;

  state->L.pressed = btns_pressed.l;
  state->L.released = btns_released.l;
  state->L.held = btns_held.l;

  state->R.pressed = btns_pressed.r;
  state->R.released = btns_released.r;
  state->R.held = btns_held.r;

  // Analog stick (left stick, X and Y axes)
  state->stick_x = inputs.stick_x;
  state->stick_y = inputs.stick_y;

  // C-stick (C-Left, C-Right, C-Up, C-Down)
  state->cstick_x = inputs.cstick_x;
  state->cstick_y = inputs.cstick_y;

  // Analog L/R-Button. Used in GameCube for analog pressure, but N64 has digital triggers
  state->analog_l = inputs.analog_l;
  state->analog_r = inputs.analog_r;
}

void map_joypad_input_n64_mouse(ButtonStates *state, joypad_port_t port)
{
  // Reset everything since for Mouse input most of it is irrelevant
  memset(state, 0, sizeof(ButtonStates));

  // Digital buttons. Mouse has only two
  joypad_buttons_t btns_pressed = joypad_get_buttons_pressed(port);
  joypad_buttons_t btns_released = joypad_get_buttons_released(port);
  joypad_buttons_t btns_held = joypad_get_buttons_held(port);

  state->A.pressed = btns_pressed.a;
  state->A.released = btns_released.a;
  state->A.held = btns_held.a;

  state->B.pressed = btns_pressed.b;
  state->B.released = btns_released.b;
  state->B.held = btns_held.b;
}

void set_rumble_supported(ControllerState *state, bool supported)
{
  state->has_rumble = supported;
}

void set_rumble_active(ControllerState *state, bool active)
{
  state->rumble_active = active;
}

void set_memory_pack_available(ControllerState *state, bool available)
{
  state->has_memory_pack = available;
}

void set_joypad_active(ControllerState *state, bool active)
{
  state->active = active;
}