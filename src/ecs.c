
#include "ecs.h"

#include <string.h>

void ecs_init(ECS *ecs)
{
  memset(ecs, 0, sizeof(ECS));
}

Entity ecs_create_entity(ECS *ecs)
{
  for (int i = 0; i < MAX_ENTITIES; i++)
  {
    if (!ecs->active_entities[i])
    {
      ecs->active_entities[i] = true;
      ecs->entity_count++;
      return i;
    }
  }
  return (Entity)-1; // No more entities available
}

void ecs_destroy_entity(ECS *ecs, Entity e)
{
  if (ecs->active_entities[e])
  {
    ecs->active_entities[e] = false;
    ecs->has_position[e] = false;
    ecs->has_velocity[e] = false;
    ecs->entity_count--;
  }
}

void ecs_add_position(ECS *ecs, Entity e, Position pos)
{
  ecs->positions[e] = pos;
  ecs->has_position[e] = true;
}

void ecs_add_velocity(ECS *ecs, Entity e, Velocity vel)
{
  ecs->velocities[e] = vel;
  ecs->has_velocity[e] = true;
}