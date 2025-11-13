#ifndef ECS_H
#define ECS_H

#include "components.h"

#include <stdbool.h>
#include <stdint.h>

#define MAX_ENTITIES 128
#define MAX_COMPONENTS 16

struct ECS;
typedef struct ECS ECS;

typedef uint32_t Entity;
typedef struct ECS
{
  bool active_entities[MAX_ENTITIES];

  Position positions[MAX_ENTITIES];
  bool has_position[MAX_ENTITIES];

  Velocity velocities[MAX_ENTITIES];
  bool has_velocity[MAX_ENTITIES];

  Model models[MAX_ENTITIES];
  bool has_model[MAX_ENTITIES];

  int entity_count;
} ECS;

void ecs_init(ECS *ecs);

Entity ecs_create_entity(ECS *const ecs);
void ecs_destroy_entity(ECS *const ecs, Entity e);

void ecs_add_position(ECS *const ecs, Entity e, Position pos);
void ecs_add_velocity(ECS *const ecs, Entity e, Velocity vel);
void ecs_add_model(ECS *const ecs, Entity e, Model model);

Position ecs_get_position(const ECS *const ecs, Entity e);
Model ecs_get_model(const ECS *const ecs, Entity e);

#endif