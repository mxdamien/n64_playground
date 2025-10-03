#ifndef ECS_H
#define ECS_H

#include "components.h"

#include <stdbool.h>
#include <stdint.h>

#define MAX_ENTITIES 128
#define MAX_COMPONENTS 16
#define MAX_CALLBACKS 10

typedef uint32_t Entity;

typedef void (*PositionCallback)(Position pos, Entity e, void *context);
typedef void (*VelocityCallback)(Velocity vel, Entity e, void *context);

typedef struct
{
  bool active_entities[MAX_ENTITIES];

  Position positions[MAX_ENTITIES];
  bool has_position[MAX_ENTITIES];
  PositionCallback position_callbacks[MAX_CALLBACKS];
  void *position_contexts[MAX_CALLBACKS];

  Velocity velocities[MAX_ENTITIES];
  bool has_velocity[MAX_ENTITIES];
  VelocityCallback velocity_callbacks[MAX_CALLBACKS];
  void *velocity_contexts[MAX_CALLBACKS];

  int entity_count;
} ECS;

void ecs_init(ECS *ecs);

Entity ecs_create_entity(ECS *const ecs);
void ecs_destroy_entity(ECS *const ecs, Entity e);
void ecs_add_position(ECS *const ecs, Entity e, Position pos);
void ecs_add_velocity(ECS *const ecs, Entity e, Velocity vel);
void ecs_register_position_callback(ECS *const ecs, PositionCallback callback, void *context);
void ecs_register_velocity_callback(ECS *const ecs, VelocityCallback callback, void *context);

Position ecs_get_position(const ECS *const ecs, Entity e);

#endif