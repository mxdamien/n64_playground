#ifndef ECS_H
#define ECS_H

#include "components.h"

#include <stdbool.h>
#include <stdint.h>

#define MAX_ENTITIES 128
#define MAX_COMPONENTS 16
#define MAX_CALLBACKS 10

struct ECS;
typedef struct ECS ECS;

typedef uint32_t Entity;

typedef void (*PositionCallback)(const ECS *const ecs, Entity e);
typedef void (*VelocityCallback)(const ECS *const ecs, Entity e);
typedef void (*ModelCallback)(const ECS *const ecs, Entity e);

typedef struct ECS
{
  bool active_entities[MAX_ENTITIES];

  Position positions[MAX_ENTITIES];
  bool has_position[MAX_ENTITIES];
  PositionCallback position_callbacks[MAX_CALLBACKS];

  Velocity velocities[MAX_ENTITIES];
  bool has_velocity[MAX_ENTITIES];
  VelocityCallback velocity_callbacks[MAX_CALLBACKS];

  Model models[MAX_ENTITIES];
  bool has_model[MAX_ENTITIES];
  ModelCallback model_callbacks[MAX_CALLBACKS];

  int entity_count;
} ECS;

// Now you can safely use 'ECS' as a typedef to the struct

void ecs_init(ECS *ecs);

Entity ecs_create_entity(ECS *const ecs);
void ecs_destroy_entity(ECS *const ecs, Entity e);
void ecs_add_position(ECS *const ecs, Entity e, Position pos);
void ecs_add_velocity(ECS *const ecs, Entity e, Velocity vel);
void ecs_add_model(ECS *const ecs, Entity e, Model model);
void ecs_register_position_callback(ECS *const ecs, PositionCallback callback);
void ecs_register_velocity_callback(ECS *const ecs, VelocityCallback callback);
void ecs_register_model_callback(ECS *const ecs, ModelCallback callback);

Position ecs_get_position(const ECS *const ecs, Entity e);
Model ecs_get_model(const ECS *const ecs, Entity e);

#endif