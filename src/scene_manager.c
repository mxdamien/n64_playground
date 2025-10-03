#include "scene_manager.h"

#include "game.h"
#include "scenes.h"
#include "scene_game.h"
#include "scene_main_menu.h"
#include "scene_scoreboard.h"
#include "inputstate.h"

void scene_manager_init(SceneManager *const scene_manager)
{
  scene_manager->current_scene = SCENE_MAIN_MENU;
}

void scene_manager_update(SceneManager *const scene_manager, GameCommand command)
{
  if (scene_manager->needs_init)
  {
    switch (scene_manager->current_scene)
    {
    case SCENE_MAIN_MENU:
      scene_init_main_menu(scene_manager);
      break;
    case SCENE_GAME:
      scene_init_game(scene_manager);
      break;
    case SCENE_SCOREBOARD:
      scene_init_scoreboard(scene_manager);
      break;
    default:
      break;
    }

    scene_manager->needs_init = false;
  }

  switch (scene_manager->current_scene)
  {
  case SCENE_MAIN_MENU:
    scene_update_main_menu(scene_manager, command);
    break;
  case SCENE_GAME:
    scene_update_game(scene_manager, command);
    break;
  case SCENE_SCOREBOARD:
    scene_update_scoreboard(scene_manager, command);
    break;
  default:
    break;
  }
}

void scene_manager_set_scene(SceneManager *const scene_manager, scene_t scene)
{
  if (scene_manager->current_scene != scene)
  {
    switch (scene_manager->current_scene)
    {
    case SCENE_GAME:
      scene_exit_game(scene_manager);
      break;
    case SCENE_MAIN_MENU:
      scene_exit_main_menu(scene_manager);
      break;
    case SCENE_SCOREBOARD:
      scene_exit_scoreboard(scene_manager);
      break;
    default:
      break;
    }

    scene_manager->previous_scene = scene_manager->current_scene;
    scene_manager->current_scene = scene;
    scene_manager->needs_init = true;
  }
}