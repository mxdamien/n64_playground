#ifndef ECS_H
#define ECS_H

#include "components.h"

#include <stdbool.h>
#include <stdint.h>

#define MAX_ENTITIES 128
#define MAX_COMPONENTS 16

typedef uint32_t Entity;

typedef struct
{
  bool active_entities[MAX_ENTITIES];

  Position positions[MAX_ENTITIES];
  bool has_position[MAX_ENTITIES];

  Velocity velocities[MAX_ENTITIES];
  bool has_velocity[MAX_ENTITIES];

  int entity_count;
} ECS;

void ecs_init(ECS *ecs);

Entity ecs_create_entity(ECS *ecs);
void ecs_destroy_entity(ECS *ecs, Entity e);
void ecs_add_position(ECS *ecs, Entity e, Position pos);
void ecs_add_velocity(ECS *ecs, Entity e, Velocity vel);

#endif