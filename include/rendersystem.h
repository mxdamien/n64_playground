#ifndef RENDERSYSTEM_H
#define RENDERSYSTEM_H

#include "camera.h"

#define MAX_RENDER_ENTITIES 100

typedef uint32_t TextureID;

typedef struct ECS ECS;

typedef struct
{
  Camera camera;
  T3DViewport viewport;
} RenderSystem;

void render_system_init(RenderSystem *const render_system);
void render_system_begin_frame(RenderSystem *const render_system);
void render_entities(RenderSystem *const render_system, ECS *const ecs);
void render_system_end_frame(RenderSystem *const render_system);

#endif