#include "rendersystem.h"
#include "ecs.h"
#include "assets.h"

#include <string.h>
#include <libdragon.h>
#include <t3d/t3d.h>
#include <t3d/t3dmath.h>
#include <t3d/t3dmodel.h>

void create_viewport(T3DViewport *viewport);
void init_camera(Camera *camera, const RenderSystemConfig *config);
void setup_lighting(const RenderSystemConfig *config);
void clear_scene();
void setup_viewport_and_camera(RenderSystem *render_system);
void set_viewport_projection(T3DViewport *viewport, const RenderSystemConfig *config);
void attach_viewport(T3DViewport *viewport);
void set_camera_view(T3DViewport *viewport, const RenderSystemConfig *config);
void render_system_begin_frame(RenderSystem *const render_system);
void render_system_end_frame(RenderSystem *const render_system);
void render_system_render_entities(RenderSystem *const render_system, ECS *const ecs);
T3DModel *render_system_get_cached_model(RenderSystem *const render_system, ECS *const ecs, Entity e);

void render_system_init(RenderSystem *const render_system, const RenderSystemConfig *config)
{
  if (render_system == NULL || config == NULL)
    return;

  memset(render_system, 0, sizeof(RenderSystem));

  render_system->config = config;

  rdpq_attach(display_get(), display_get_zbuf());
  setup_viewport_and_camera(render_system);
  setup_lighting(render_system->config);
  clear_scene();
  rdpq_detach_show();
}

void setup_viewport_and_camera(RenderSystem *const render_system)
{
  create_viewport(&render_system->viewport);
  init_camera(&render_system->camera, render_system->config);
  set_viewport_projection(&render_system->viewport, render_system->config);
  attach_viewport(&render_system->viewport);
  set_camera_view(&render_system->viewport, render_system->config);
}

void create_viewport(T3DViewport *viewport)
{
  if (viewport == NULL)
    return;

  *viewport = t3d_viewport_create();
}

void init_camera(Camera *camera, const RenderSystemConfig *config)
{
  if (camera == NULL || config == NULL)
    return;

  *camera = (Camera){
      .position = config->camera_position,
      .target = config->camera_target,
      .up = config->camera_up,
      .fov = T3D_DEG_TO_RAD(config->camera_fov),
      .near_plane = config->camera_near_plane,
      .far_plane = config->camera_far_plane,
  };
}

void set_viewport_projection(T3DViewport *viewport, const RenderSystemConfig *config)
{
  if (config == NULL)
    return;

  t3d_viewport_set_projection(viewport,
                              T3D_DEG_TO_RAD(config->viewport_fov),
                              config->viewport_near_plane,
                              config->viewport_far_plane);
}

void attach_viewport(T3DViewport *viewport)
{
  t3d_viewport_attach(viewport);
}

void set_camera_view(T3DViewport *viewport, const RenderSystemConfig *config)
{
  if (config == NULL)
    return;

  t3d_viewport_look_at(viewport,
                       &config->camera_position,
                       &config->camera_target,
                       &(T3DVec3){{0, 1, 0}});
}

void setup_lighting(const RenderSystemConfig *config)
{
  if (config == NULL)
    return;

  // Set ambient light
  t3d_light_set_ambient(config->ambient_color);

  // Set directional light
  T3DVec3 lightDirVec = config->light_direction;
  t3d_vec3_norm(&lightDirVec);
  t3d_light_set_directional(0, config->directional_color, &lightDirVec);

  // Set the number of lights
  t3d_light_set_count(1);
}

void render_system_update_frame(RenderSystem *system, ECS *ecs)
{
  render_system_begin_frame(system);
  render_system_render_entities(system, ecs);
  render_system_end_frame(system);
}

void render_system_begin_frame(RenderSystem *const render_system)
{
  if (render_system == NULL)
    return;

  rdpq_attach(display_get(), display_get_zbuf());
  t3d_frame_start();
  t3d_viewport_attach(&render_system->viewport);
  clear_scene();
  rspq_block_begin();
}

void clear_scene()
{
  t3d_screen_clear_color(RGBA32(0, 0, 0, 0xFF));
  t3d_screen_clear_depth();
}

void render_system_render_entities(RenderSystem *const render_system, ECS *const ecs)
{
  if (render_system == NULL || ecs == NULL)
    return;

  for (int e = 0; e < MAX_ENTITIES; e++)
  {
    if (!ecs->active_entities[e])
      continue;

    T3DModel *model = render_system_get_cached_model(render_system, ecs, e);
    if (model == NULL)
      return;

    Position pos = ecs_get_position(ecs, e);
    Rotation rot = ecs_get_rotation(ecs, e);

    T3DMat4 mat;
    t3d_mat4_identity(&mat);

    float scale = render_system->config->entity_scale;
    t3d_mat4_from_srt_euler(
        &mat,
        (float[3]){scale, scale, scale},
        (float[3]){rot.x, rot.y, rot.z},
        (float[3]){pos.x, pos.y, pos.z});

    T3DMat4FP *fp = malloc_uncached(sizeof(T3DMat4FP));
    t3d_mat4_to_fixed(fp, &mat);
    t3d_matrix_push(fp);
    t3d_model_draw(model);
    t3d_matrix_pop(1);
  }
}

T3DModel *render_system_get_cached_model(RenderSystem *const render_system, ECS *const ecs, Entity e)
{
  Model modelID = ecs_get_model(ecs, e);
  if (render_system->model_cache[modelID])
    return render_system->model_cache[modelID];

  const char *fileName = get_model_path(modelID);
  if (strcmp(fileName, ID_ASSET_INVALID) == 0)
    return NULL;

  render_system->model_cache[modelID] = t3d_model_load(fileName);
  return render_system->model_cache[modelID];
}

void render_system_end_frame(RenderSystem *const render_system)
{
  if (render_system == NULL)
    return;
  rspq_block_t *dplDraw = rspq_block_end();
  rspq_block_run(dplDraw);
  rdpq_detach_show();
}

void render_system_cleanup(RenderSystem *const render_system)
{
  if (render_system == NULL)
    return;

  // Free cached models
  for (int i = 0; i < MAX_RENDER_ENTITIES; i++)
  {
    if (render_system->model_cache[i] != NULL)
    {
      t3d_model_free(render_system->model_cache[i]);
      render_system->model_cache[i] = NULL;
    }
  }

  memset(render_system, 0, sizeof(RenderSystem));
}