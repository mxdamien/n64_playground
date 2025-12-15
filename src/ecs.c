#include "ecs.h"
#include <string.h>

void ecs_init(ECS *ecs)
{
  memset(ecs, 0, sizeof(ECS));
}

Entity ecs_create_entity(ECS *const ecs)
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

void ecs_destroy_entity(ECS *const ecs, Entity e)
{
  if (e >= MAX_ENTITIES)
    return;

  if (ecs->active_entities[e])
  {
    ecs->active_entities[e] = false;
    ecs->has_position[e] = false;
    ecs->has_velocity[e] = false;
    ecs->has_model[e] = false;
    ecs->has_rotation[e] = false;
    ecs->entity_count--;
  }
}

void ecs_add_position(ECS *const ecs, Entity e, Position pos)
{
  if (e >= MAX_ENTITIES)
    return;

  ecs->positions[e] = pos;
  ecs->has_position[e] = true;
}

void ecs_add_velocity(ECS *const ecs, Entity e, Velocity vel)
{
  if (e >= MAX_ENTITIES)
    return;

  ecs->velocities[e] = vel;
  ecs->has_velocity[e] = true;
}

void ecs_add_model(ECS *const ecs, Entity e, Model model)
{
  if (e >= MAX_ENTITIES)
    return;

  ecs->models[e] = model;
  ecs->has_model[e] = true;
}

Position ecs_get_position(const ECS *const ecs, Entity e)
{
  if (e >= MAX_ENTITIES || !ecs->has_position[e])
    return (Position){0.0f, 0.0f, 0.0f};

  return ecs->positions[e];
}

Model ecs_get_model(const ECS *const ecs, Entity e)
{
  if (e >= MAX_ENTITIES || !ecs->has_model[e])
    return 0;

  return ecs->models[e];
}

void ecs_add_rotation(ECS *const ecs, Entity e, Rotation tilt)
{
  if (e >= MAX_ENTITIES)
    return;

  ecs->rotations[e] = tilt;
  ecs->has_rotation[e] = true;
}

Rotation ecs_get_rotation(ECS *const ecs, Entity e)
{
  if (e >= MAX_ENTITIES || !ecs->has_rotation[e])
    return (Rotation){0.0f, 0.0f, 0.0f};

  return ecs->rotations[e];
}
