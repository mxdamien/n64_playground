#ifndef GAME_H
#define GAME_H

#include "inputstate.h"
#include "scene_manager.h"
#include "ecs.h"

#include <stdint.h>
#include <libdragon.h>

typedef struct
{
  InputState input_state;
  SceneManager scene_manager;
} GameContext;

void game_loop(GameContext *const game);

#endif