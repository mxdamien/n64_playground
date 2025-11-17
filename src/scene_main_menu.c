#include "scene_main_menu.h"

#include "draw_string.h"

#include <libdragon.h>
#include <string.h>

static SceneMainMenuState scene_state;
typedef enum
{
  MENU_START_GAME,
  MENU_SETTINGS,
  MENU_QUIT,
  MENU_COUNT
} MenuOption;

#define MENU_HEADER "Boing Ball"
#define MENU_ITEM_START_GAME "Start Game"
#define MENU_ITEM_SCORE "Scoreboard"
#define MENU_ITEM_QUIT "Quit"

#define MENU_ITEM_COLOR_HIGHLIGHT 0xFFFFFF00
#define MENU_ITEM_COLOR_NORMAL 0xFFFFFFFF

void scene_init_main_menu(SceneManager *scene_manager)
{
  ecs_init(&scene_state.ecs);

  surface_t *disp = display_get();
  graphics_fill_screen(disp, 0x0);
  graphics_set_color(0xFFFFFFFF, 0x0);

  for (int i = 0; i < MENU_COUNT; i++)
  {
    int y_position = 80 + i * 20; // Vertical spacing
    const char *menu_text = NULL;

    switch (i)
    {
    case MENU_START_GAME:
      menu_text = MENU_ITEM_START_GAME;
      break;
    case MENU_SETTINGS:
      menu_text = MENU_ITEM_SCORE;
      break;
    case MENU_QUIT:
      menu_text = MENU_ITEM_QUIT;
      break;
    }

    print_centered_text(disp, y_position, menu_text);
  }

  display_show(disp);
}

void scene_update_main_menu(SceneManager *scene_manager, GameCommand command)
{
  static int selected_item = 0;

  if (command == COMMAND_SELECT)
  {
    switch (selected_item)
    {
    case MENU_START_GAME:
      scene_manager_set_scene(scene_manager, SCENE_GAME);
      break;
    case MENU_SETTINGS:
      scene_manager_set_scene(scene_manager, SCENE_SCOREBOARD);
      break;
    case MENU_QUIT:
      // How can we actually quit the game?
      break;
    }
  }

  if (command == COMMAND_UP)
  {
    selected_item = (selected_item + MENU_COUNT - 1) % MENU_COUNT;
  }
  else if (command == COMMAND_DOWN)
  {
    selected_item = (selected_item + 1) % MENU_COUNT;
  }

  for (int i = 0; i < MENU_COUNT; i++)
  {
    int y_position = 80 + i * 20;
    const char *menu_text = NULL;

    switch (i)
    {
    case MENU_START_GAME:
      menu_text = MENU_ITEM_START_GAME;
      break;
    case MENU_SETTINGS:
      menu_text = MENU_ITEM_SCORE;
      break;
    case MENU_QUIT:
      menu_text = MENU_ITEM_QUIT;
      break;
    }

    if (i == selected_item)
    {
      graphics_set_color(MENU_ITEM_COLOR_HIGHLIGHT, 0x0);
    }
    else
    {
      graphics_set_color(MENU_ITEM_COLOR_NORMAL, 0x0);
    }
    surface_t *disp = display_get();
    print_centered_text(disp, y_position, menu_text);
    display_show(disp);
  }
}

void scene_exit_main_menu()
{
  surface_t *disp = display_get();
  graphics_fill_screen(disp, 0x0);
  display_show(disp);
  memset(&scene_state, 0, sizeof(SceneMainMenuState));
}