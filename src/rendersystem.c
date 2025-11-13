#include "rendersystem.h"
#include "ecs.h"
#include "assets.h"

#include <string.h>
#include <libdragon.h>
#include <t3d/t3d.h>
#include <t3d/t3dmath.h>
#include <t3d/t3dmodel.h>

T3DViewport viewport;

color_t get_rainbow_color(float s)
{
  float r = fm_sinf(s + 0.0f) * 127.0f + 128.0f;
  float g = fm_sinf(s + 2.0f) * 127.0f + 128.0f;
  float b = fm_sinf(s + 4.0f) * 127.0f + 128.0f;
  return RGBA32(r, g, b, 255);
}

void render_system_init(RenderSystem *const render_system)
{
  memset(render_system, 0, sizeof(RenderSystem));
}

void render_entities(ECS *const ecs)
{
  for (int e = 0; e < MAX_ENTITIES; ++e)
  {
    if (ecs->active_entities[e])
    {
      Model modelId = ecs_get_model(ecs, e);
      const char *fileName = GetModelPath(modelId);

      Position position = ecs_get_position(ecs, e);

      if (strcmp(fileName, ID_ASSET_INVALID) == 0)
        return;

      T3DViewport viewport = t3d_viewport_create();

      T3DMat4 modelMat; // matrix for our model, this is a "normal" float matrix
      t3d_mat4_identity(&modelMat);

      // Now allocate a fixed-point matrix, this is what t3d uses internally.
      // Note: this gets DMA'd to the RSP, so it needs to be uncached.
      // If you can't allocate uncached memory, remember to flush the cache after writing to it instead.
      T3DMat4FP *modelMatFP = malloc_uncached(sizeof(T3DMat4FP));

      const T3DVec3 camPos = {{0, 10.0f, 40.0f}};
      const T3DVec3 camTarget = {{0, 0, 0}};

      uint8_t colorAmbient[4] = {80, 80, 100, 0xFF};
      uint8_t colorDir[4] = {0xEE, 0xAA, 0xAA, 0xFF};

      T3DVec3 lightDirVec = {{-1.0f, 1.0f, 1.0f}};
      t3d_vec3_norm(&lightDirVec);

      // Load a model-file, this contains the geometry and some metadata
      T3DModel *model = t3d_model_load(fileName);

      float rotAngle = 0.0f;
      rspq_block_t *dplDraw = NULL;

      // for (;;)
      {
        // ======== Update ======== //
        rotAngle -= 0.05f - position.x / 100.0;
        float modelScale = 0.1f;

        t3d_viewport_set_projection(&viewport, T3D_DEG_TO_RAD(85.0f), 10.0f, 150.0f);
        t3d_viewport_look_at(&viewport, &camPos, &camTarget, &(T3DVec3){{0, 1, 0}});

        // slowly rotate model, for more information on matrices and how to draw objects
        // see the example: "03_objects"
        t3d_mat4_from_srt_euler(&modelMat,
                                (float[3]){modelScale, modelScale, modelScale},
                                (float[3]){0.0f, rotAngle * 0.2f, rotAngle},
                                (float[3]){0, 0, 0});

        t3d_mat4_to_fixed(modelMatFP, &modelMat);

        // ======== Draw ======== //
        rdpq_attach(display_get(), display_get_zbuf());
        t3d_frame_start();
        t3d_viewport_attach(&viewport);

        t3d_screen_clear_color(RGBA32(100, 80, 80, 0xFF));
        t3d_screen_clear_depth();

        t3d_light_set_ambient(colorAmbient);
        t3d_light_set_directional(0, colorDir, &lightDirVec);
        t3d_light_set_count(1);

        // you can use the regular rdpq_* functions with t3d.
        // In this example, the colored-band in the 3d-model is using the prim-color,
        // even though the model is recorded, you change it here dynamically.
        rdpq_set_prim_color(get_rainbow_color(rotAngle * 0.42f));

        if (!dplDraw)
        {
          rspq_block_begin();

          t3d_matrix_push(modelMatFP);
          // Draw the model, material settings (e.g. textures, color-combiner) are handled internally
          t3d_model_draw(model);
          t3d_matrix_pop(1);
          dplDraw = rspq_block_end();
        }

        // for the actual draw, you can use the generic rspq-api.
        rspq_block_run(dplDraw);

        rdpq_detach_show();
      }

      // t3d_destroy();
    }
  }
}