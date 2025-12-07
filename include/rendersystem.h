#ifndef RENDERSYSTEM_H
#define RENDERSYSTEM_H

#include "camera.h"
#include "t3d/t3dmodel.h"

#define MAX_RENDER_ENTITIES 100

typedef uint32_t TextureID;

typedef struct ECS ECS;

typedef struct
{
  T3DVec3 camera_position;
  T3DVec3 camera_target;
  T3DVec3 camera_up;
  float camera_fov;
  float camera_near_plane;
  float camera_far_plane;

  float viewport_fov;
  float viewport_near_plane;
  float viewport_far_plane;

  uint8_t ambient_color[4];
  uint8_t directional_color[4];
  T3DVec3 light_direction;

  float entity_scale;
  uint32_t clear_color;
} RenderSystemConfig;

typedef struct
{
  Camera camera;
  T3DViewport viewport;
  T3DModel *model_cache[MAX_RENDER_ENTITIES];
  const RenderSystemConfig *config;
} RenderSystem;

void render_system_init(RenderSystem *const render_system, const RenderSystemConfig *config);
void render_system_update_frame(RenderSystem *system, ECS *ecs);
void render_system_cleanup(RenderSystem *const render_system);

#endif