#include "rendersystem.h"
#include "ecs.h"
#include "assets.h"

#include <string.h>
#include <libdragon.h>
#include <t3d/t3d.h>
#include <t3d/t3dmath.h>
#include <t3d/t3dmodel.h>

void create_viewport(T3DViewport *viewport);
void init_camera(Camera *camera);
void setup_lighting();
void clear_scene();
void setup_viewport_and_camera(RenderSystem *render_system);
void set_viewport_projection(T3DViewport *viewport);
void attach_viewport(T3DViewport *viewport);
void set_camera_view(T3DViewport *viewport);
void render_system_begin_frame(RenderSystem *const render_system);
void render_system_end_frame(RenderSystem *const render_system);
void render_system_render_entities(RenderSystem *const render_system, ECS *const ecs);
T3DModel *render_system_get_cached_model(RenderSystem *const render_system, ECS *const ecs, Entity e);

void render_system_init(RenderSystem *const render_system)
{
  if (render_system == NULL)
    return;

  memset(render_system, 0, sizeof(RenderSystem));

  rdpq_attach(display_get(), display_get_zbuf());
  setup_viewport_and_camera(render_system);
  clear_scene();
  setup_lighting();
  rdpq_detach_show();
}

void setup_viewport_and_camera(RenderSystem *render_system)
{
  create_viewport(&render_system->viewport);
  init_camera(&render_system->camera);
  set_viewport_projection(&render_system->viewport);
  attach_viewport(&render_system->viewport);
  set_camera_view(&render_system->viewport);
}

void create_viewport(T3DViewport *viewport)
{
  if (viewport == NULL)
    return;

  *viewport = t3d_viewport_create();
}

void init_camera(Camera *camera)
{
  if (camera == NULL)
    return;

  *camera = (Camera){
      .position = {{0, 10, 40}},
      .target = {{0, 0, 0}},
      .up = {{0, 1, 0}},
      .fov = T3D_DEG_TO_RAD(85.0f),
      .near_plane = 1.0f,
      .far_plane = 200.0f,
  };
}

void set_viewport_projection(T3DViewport *viewport)
{
  const float fov = T3D_DEG_TO_RAD(85.0f);
  const float nearPlane = 10.0f;
  const float farPlane = 150.0f;

  t3d_viewport_set_projection(viewport, fov, nearPlane, farPlane);
}

void attach_viewport(T3DViewport *viewport)
{
  t3d_viewport_attach(viewport);
}

void set_camera_view(T3DViewport *viewport)
{
  const T3DVec3 camPos = {{0, 10.0f, 40.0f}};
  const T3DVec3 camTarget = {{0, 0, 0}};

  t3d_viewport_look_at(viewport, &camPos, &camTarget, &(T3DVec3){{0, 1, 0}});
}

void clear_scene()
{
  t3d_screen_clear_color(RGBA32(0, 0, 0, 0xFF));
  t3d_screen_clear_depth();
}

void setup_lighting()
{
  uint8_t colorAmbient[4] = {80, 80, 100, 0xFF};
  uint8_t colorDir[4] = {0xEE, 0xAA, 0xAA, 0xFF};
  T3DVec3 lightDirVec = {{-1.0f, 1.0f, 1.0f}};
  t3d_vec3_norm(&lightDirVec);
  t3d_light_set_ambient(colorAmbient);
  t3d_light_set_directional(0, colorDir, &lightDirVec);
  t3d_light_set_count(1);
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

void render_system_render_entities(RenderSystem *const render_system, ECS *const ecs)
{
  if (render_system == NULL)
    return;

  if (ecs == NULL)
    return;

  for (int e = 0; e < MAX_ENTITIES; e++)
  {
    if (!ecs->active_entities[e])
      continue;

    T3DModel *model = render_system_get_cached_model(render_system, ecs, e);
    if (model == NULL)
      return;

    Position pos = ecs_get_position(ecs, e);

    T3DMat4 mat;
    t3d_mat4_identity(&mat);

    float scale = 0.1f;
    t3d_mat4_from_srt_euler(
        &mat,
        (float[3]){scale, scale, scale},
        (float[3]){0, pos.y, 0},
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

  memset(render_system, 0, sizeof(RenderSystem));
}
