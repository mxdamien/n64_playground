#ifndef INPUTSTATE_H
#define INPUTSTATE_H

#define CONTROLLER_MAX 4

#include <stdint.h>
#include <stdbool.h>

typedef struct
{
  bool pressed;
  bool released;
  bool held;
} ButtonState;

typedef struct
{
  ButtonState A;     // A button
  ButtonState B;     // B button
  ButtonState Start; // Start button
  ButtonState Z;     // Z button
  ButtonState Left;  // Left D-pad
  ButtonState Right; // Right D-pad
  ButtonState Up;    // Up D-pad
  ButtonState Down;  // Down D-pad
  ButtonState L;     // L-Button
  ButtonState R;     // R-Button
  int8_t stick_x;    // Stick X Position
  int8_t stick_y;    // Stick Y Position
  int8_t cstick_x;   // C-Stick X Position for Gamecube. C-Left and C-Right for N64 (-76=C-Left, +76=C-Right)
  int8_t cstick_y;   // C-Stick Y Position for Gamecube. C-Down and C-UP for N64 (-76=C-Down, +76=C-Up)
  int8_t analog_l;   // Analog L-Button for Gamecube. Regular L-Button for N64 (0=unpressed, 200=pressed)
  int8_t analog_r;   // Analog R-Button for Gamecube. Regular R-Button for N64 (0=unpressed, 200=pressed)
} ButtonStates;

typedef struct
{
  ButtonStates buttons;             // Button states for the controller
  unsigned int active : 1;          // Flag to indicate if the controller is active (connected)
  unsigned int has_memory_pack : 1; // Flag to indicate if the controller has a memory card
  unsigned int has_rumble : 1;      // Flag to indicate if the controller has rumble functionality
  unsigned int rumble_active : 1;   // Flags to indicate if rumb pack is active
} ControllerState;

typedef struct
{
  ControllerState controller[CONTROLLER_MAX];
} InputState;

#endif