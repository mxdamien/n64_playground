#ifndef RENDERSYSTEM_H
#define RENDERSYSTEM_H

#include "ecs.h"

#define MAX_RENDER_ENTITIES 100

typedef uint32_t TextureID;

typedef struct
{
  Entity entities[MAX_RENDER_ENTITIES];
  uint32_t texture_ids[MAX_RENDER_ENTITIES];
  uint32_t entity_count;
} RenderSystem;

void render_system_init(RenderSystem *const render_system);
void render_system_register_callbacks(ECS *const ecs, RenderSystem *const render_system);
void render_system_register_entity(RenderSystem *const ender_system, Entity e, TextureID texture_id);
void render_system_update(const RenderSystem *const render_system);

#endif