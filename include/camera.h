
#ifndef CAMERA_H
#define CAMERA_H

#include <t3d/t3d.h>

typedef struct
{
  T3DVec3 position;
  T3DVec3 target;
  T3DVec3 up;
  float fov;
  float near_plane;
  float far_plane;
} Camera;

#endif