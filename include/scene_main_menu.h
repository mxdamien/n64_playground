#ifndef SCENE_MAIN_MENU_H
#define SCENE_MAIN_MENU_H

#include "scene_manager.h"
#include "inputstate.h"
#include "ecs.h"

typedef struct
{
  ECS ecs;
} SceneMainMenuState;

void scene_init_main_menu(SceneManager *scene_manager);
void scene_update_main_menu(SceneManager *scene_manager, GameCommand command);
void scene_exit_main_menu();

#endif