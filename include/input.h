#ifndef INPUT_H
#define INPUT_H

#include "inputstate.h"

#include <libdragon.h>
#include <stdbool.h>

void input_init(InputState *input_state);

void input_read(InputState *input_state);
void map_joypad_input_n64_standard_and_gamecube(ButtonStates *state, joypad_port_t port);
void map_joypad_input_n64_mouse(ButtonStates *state, joypad_port_t port);
void set_rumble_supported(ControllerState *state, bool supported);
void set_rumble_active(ControllerState *state, bool active);
void set_joypad_active(ControllerState *state, bool active);
bool joypad_get_memory_pack_available(joypad_port_t port);
void set_memory_pack_available(ControllerState *state, bool available);

#endif