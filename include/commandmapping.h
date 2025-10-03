#ifndef COMMANDMAPPING_H
#define COMMANDMAPPING_H

#include "commands.h"
#include "scenes.h"
#include "inputstate.h"

GameCommand scene_input_to_command_mapping(scene_t scene, const InputState *const inputState);

#endif