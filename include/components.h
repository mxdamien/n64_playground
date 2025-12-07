#ifndef COMPONENTS_H
#define COMPONENTS_H

#include "vec3.h"
#include "stdint.h"

typedef vec3 Position;
typedef vec3 Velocity;
typedef uint32_t Model;

typedef struct
{
  float current_tilt_angle;
  float max_tilt_angle;
} Tilt;

#endif