#include "ecs.h"

#include <string.h>

void trigger_position_callbacks(const ECS *ecs, Entity e);
void trigger_velocity_callbacks(const ECS *ecs, Entity e);
void trigger_model_callbacks(const ECS *ecs, Entity e);

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
  if (ecs->active_entities[e])
  {
    ecs->active_entities[e] = false;
    ecs->has_position[e] = false;
    ecs->has_velocity[e] = false;
    ecs->entity_count--;
  }
}

void ecs_add_position(ECS *const ecs, Entity e, Position pos)
{
  ecs->positions[e] = pos;
  ecs->has_position[e] = true;
  trigger_position_callbacks(ecs, e);
}

void ecs_add_velocity(ECS *const ecs, Entity e, Velocity vel)
{
  ecs->velocities[e] = vel;
  ecs->has_velocity[e] = true;
  trigger_velocity_callbacks(ecs, e);
}

void ecs_add_model(ECS *const ecs, Entity e, Model model)
{
  ecs->models[e] = model;
  ecs->has_model[e] = true;
  trigger_model_callbacks(ecs, e);
}

void ecs_register_velocity_callback(ECS *const ecs, VelocityCallback callback)
{
  for (int i = 0; i < MAX_CALLBACKS; i++)
  {
    if (ecs->velocity_callbacks[i] == NULL)
    {
      ecs->velocity_callbacks[i] = callback;
      return;
    }
  }
}

void trigger_velocity_callbacks(const ECS *const ecs, Entity e)
{
  for (int i = 0; i < MAX_CALLBACKS; i++)
  {
    if (ecs->velocity_callbacks[i])
    {
      ecs->velocity_callbacks[i](ecs, e);
    }
  }
}

void ecs_register_position_callback(ECS *const ecs, PositionCallback callback)
{
  if (!callback)
    return;

  for (int i = 0; i < MAX_CALLBACKS; i++)
  {
    if (ecs->position_callbacks[i] == NULL)
    {
      ecs->position_callbacks[i] = callback;
      return;
    }
  }
}

void trigger_position_callbacks(const ECS *const ecs, Entity e)
{
  for (int i = 0; i < MAX_CALLBACKS; i++)
  {
    if (ecs->position_callbacks[i])
    {
      ecs->position_callbacks[i](ecs, e);
    }
  }
}

Position ecs_get_position(const ECS *const ecs, Entity e)
{
  return ecs->positions[e];
}

void ecs_register_model_callback(ECS *const ecs, ModelCallback callback)
{
  if (!callback)
    return;

  for (int i = 0; i < MAX_CALLBACKS; i++)
  {
    if (ecs->model_callbacks[i] == NULL)
    {
      ecs->model_callbacks[i] = callback;
      return;
    }
  }
}

void trigger_model_callbacks(const ECS *const ecs, Entity e)
{
  for (int i = 0; i < MAX_CALLBACKS; i++)
  {
    if (ecs->model_callbacks[i])
    {
      ecs->model_callbacks[i](ecs, e);
    }
  }
}

Model ecs_get_model(const ECS *const ecs, Entity e)
{
  return ecs->models[e];
}