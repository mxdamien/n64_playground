#ifndef SCENE_GAME_H
#define SCENE_GAME_H

#include "scene_manager.h"
#include "inputstate.h"
#include "ecs.h"
#include "rendersystem.h"

typedef struct
{
  ECS ecs;
  RenderSystem renderSystem;
} SceneGameState;

void scene_init_game(SceneManager *scene_manager);
void scene_update_game(SceneManager *scene_manager, GameCommand command);
void scene_exit_game();

#endif