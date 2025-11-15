#include "rendersystem.h"
#include "ecs.h"
#include "assets.h"

#include <string.h>
#include <libdragon.h>
#include <t3d/t3d.h>
#include <t3d/t3dmath.h>
#include <t3d/t3dmodel.h>

T3DViewport viewport;

void render_system_init(RenderSystem *const render_system)
{
  memset(render_system, 0, sizeof(RenderSystem));

  viewport = t3d_viewport_create();
}

void render_entities(ECS *const ecs)
{
  rspq_block_t *dplDraw = NULL;
  rdpq_attach(display_get(), display_get_zbuf());
  t3d_frame_start();
  t3d_viewport_attach(&viewport);

  const T3DVec3 camPos = {{0, 10.0f, 40.0f}};
  const T3DVec3 camTarget = {{0, 0, 0}};

  t3d_viewport_set_projection(&viewport, T3D_DEG_TO_RAD(85.0f), 10.0f, 150.0f);
  t3d_viewport_look_at(&viewport, &camPos, &camTarget, &(T3DVec3){{0, 1, 0}});

  for (int e = 0; e < MAX_ENTITIES; ++e)
  {
    if (ecs->active_entities[e])
    {
      const char *fileName = GetModelPath(ecs_get_model(ecs, e));
      if (strcmp(fileName, ID_ASSET_INVALID) == 0)
        return;

      Position position = ecs_get_position(ecs, e);

      T3DMat4 modelMat;
      t3d_mat4_identity(&modelMat);

      uint8_t colorAmbient[4] = {80, 80, 100, 0xFF};
      uint8_t colorDir[4] = {0xEE, 0xAA, 0xAA, 0xFF};

      T3DVec3 lightDirVec = {{-1.0f, 1.0f, 1.0f}};
      t3d_vec3_norm(&lightDirVec);

      T3DModel *model = t3d_model_load(fileName);

      float rotAngle = 0.0f;
      rotAngle -= 0.05f - position.x / 50.0;
      float modelScale = 0.1f;

      t3d_mat4_from_srt_euler(&modelMat,
                              (float[3]){modelScale, modelScale, modelScale},
                              (float[3]){0.0f, rotAngle * 0.2f, rotAngle},
                              (float[3]){0, 0, 0});

      T3DMat4FP *modelMatFP = malloc_uncached(sizeof(T3DMat4FP));
      t3d_mat4_to_fixed(modelMatFP, &modelMat);

      t3d_screen_clear_color(RGBA32(0, 0, 0, 0xFF));
      t3d_screen_clear_depth();

      t3d_light_set_ambient(colorAmbient);
      t3d_light_set_directional(0, colorDir, &lightDirVec);
      t3d_light_set_count(1);

      rspq_block_begin();

      t3d_matrix_push(modelMatFP);
      t3d_model_draw(model);
      t3d_matrix_pop(1);

      dplDraw = rspq_block_end();
      rspq_block_run(dplDraw);
      t3d_model_free(model);
    }
  }

  rdpq_detach_show();
}