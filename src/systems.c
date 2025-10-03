#include "systems.h"
#include "ecs.h"

void system_movement(ECS *ecs)
{
  for (int i = 0; i < MAX_ENTITIES; i++)
  {
    if (ecs->active_entities[i] &&
        ecs->has_position[i] &&
        ecs->has_velocity[i])
    {
      ecs->positions[i].x += ecs->velocities[i].x;
      ecs->positions[i].y += ecs->velocities[i].y;
      ecs->positions[i].z += ecs->velocities[i].z;
    }
  }
}