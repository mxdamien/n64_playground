#ifndef SCENE_SCOREBOARD_H
#define SCENE_SCOREBOARD_H

#include "scene_manager.h"
#include "inputstate.h"
#include "ecs.h"
#include "rendersystem.h"

typedef struct
{
  ECS ecs;
  RenderSystem renderSystem;
} SceneScoreboardState;

void scene_init_scoreboard(SceneManager *scene_manager);
void scene_update_scoreboard(SceneManager *scene_manager, GameCommand command);
void scene_exit_scoreboard();

#endif