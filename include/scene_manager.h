#ifndef SCENE_MANAGER_H
#define SCENE_MANAGER_H

#include "inputstate.h"
#include "scenes.h"
#include "ecs.h"
#include "commands.h"

#include <stdint.h>

typedef struct
{
  scene_t current_scene;
  scene_t previous_scene;
  bool needs_init;
} SceneManager;

void scene_manager_init(SceneManager *scene_manager);
void scene_manager_update(SceneManager *scene_manager, GameCommand command);
void scene_manager_set_scene(SceneManager *scene_manager, scene_t scene);

#endif
