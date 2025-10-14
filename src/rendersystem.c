#include "rendersystem.h"
#include "ecs.h"

#include <string.h>
#include <libdragon.h>
#include <t3d/t3d.h>

void render_system_init(RenderSystem *const render_system)
{
  memset(render_system, 0, sizeof(RenderSystem));
}

void render_system_register_entity(RenderSystem *const render_system, Entity e, uint32_t texture_id)
{
  if (render_system->entity_count < MAX_RENDER_ENTITIES)
  {
    render_system->entities[render_system->entity_count] = e;
    render_system->texture_ids[render_system->entity_count] = texture_id;
    render_system->entity_count++;
  }
}

void on_position_updated(Position pos, Entity e, void *context)
{
  if (context == NULL)
    return;

  T3DMat4 modelMat; // matrix for our model, this is a "normal" float matrix
  t3d_mat4_identity(&modelMat);

  // Now allocate a fixed-point matrix, this is what t3d uses internally.
  T3DMat4FP *modelMatFP = malloc_uncached(sizeof(T3DMat4FP));

  const T3DVec3 camPos = {{0, 0, -18}};
  const T3DVec3 camTarget = {{0, 0, 0}};

  uint8_t colorAmbient[4] = {50, 50, 50, 0xFF};
  uint8_t colorDir[4] = {0xFF, 0xFF, 0xFF, 0xFF};

  T3DVec3 lightDirVec = {{0.0f, 0.0f, 1.0f}};
  t3d_vec3_norm(&lightDirVec);

  // Allocate vertices (make sure to have an uncached pointer before passing it to the API!)
  T3DVertPacked *vertices = malloc_uncached(sizeof(T3DVertPacked) * 2);

  uint16_t norm = t3d_vert_pack_normal(&(T3DVec3){{0, 0, 1}}); // normals are packed in a 5.6.5 format
  vertices[0] = (T3DVertPacked){
      .posA = {-16, -16, 0},
      .rgbaA = 0xFF0000,
      .normA = norm,
      .posB = {16, -16, 0},
      .rgbaB = 0x00FF00,
      .normB = norm,
  };
  vertices[1] = (T3DVertPacked){
      .posA = {16, 16, 0},
      .rgbaA = 0x0000FF,
      .normA = norm,
      .posB = {-16, 16, 0},
      .rgbaB = 0xFF00FF,
      .normB = norm,
  };

  float rotAngle = 0.0f;
  T3DVec3 rotAxis = {{-1.0f, 2.5f, 0.25f}};
  t3d_vec3_norm(&rotAxis);

  // create a viewport, this defines the section to draw to (by default the whole screen)
  T3DViewport viewport = t3d_viewport_create();

  rspq_block_t *dplDraw = NULL;

  // for (;;)
  {
    // ======== Update ======== //
    rotAngle += 45.0f;

    // Set up viewport settings
    t3d_viewport_set_projection(&viewport, T3D_DEG_TO_RAD(85.0f), 10.0f, 100.0f);
    t3d_viewport_look_at(&viewport, &camPos, &camTarget, &(T3DVec3){{0, 1, 0}});

    // Model-Matrix, t3d offers some basic matrix functions
    t3d_mat4_identity(&modelMat);

    // Rotate the object
    t3d_mat4_rotate(&modelMat, &rotAxis, rotAngle);
    t3d_mat4_scale(&modelMat, 0.4f, 0.4f, 0.4f);

    // Apply the position (shift the object to pos)
    t3d_mat4_translate(&modelMat, pos.x, pos.y, pos.z); // Translate the object by pos.x, pos.y, pos.z

    // Convert to fixed-point format
    t3d_mat4_to_fixed(modelMatFP, &modelMat);

    // ======== Draw (3D) ======== //
    rdpq_attach(display_get(), display_get_zbuf()); // set the target to draw to
    t3d_frame_start();                              // call this once per frame at the beginning of your draw function

    t3d_viewport_attach(&viewport); // now use the viewport, this applies proj/view matrices and sets scissoring

    rdpq_mode_combiner(RDPQ_COMBINER_SHADE);
    t3d_screen_clear_color(RGBA32(100, 0, 100, 0));
    t3d_screen_clear_depth();

    t3d_light_set_ambient(colorAmbient);
    t3d_light_set_directional(0, colorDir, &lightDirVec);
    t3d_light_set_count(1);

    t3d_state_set_drawflags(T3D_FLAG_SHADED | T3D_FLAG_DEPTH);

    if (!dplDraw)
    {
      rspq_block_begin();

      t3d_matrix_push(modelMatFP);   // Matrix load can be recorded as they DMA the data in internally
      t3d_vert_load(vertices, 0, 4); // load 4 vertices...
      t3d_matrix_pop(1);             // ...and pop the matrix
      t3d_tri_draw(0, 1, 2);         // ...then draw 2 triangles
      t3d_tri_draw(2, 3, 0);

      t3d_tri_sync(); // sync after drawing the triangles

      dplDraw = rspq_block_end();
    }

    rspq_block_run(dplDraw);

    rdpq_detach_show();
  }

  // t3d_destroy();
}

void render_system_register_callbacks(ECS *const ecs, RenderSystem *const render_system)
{
  ecs_register_position_callback(ecs, on_position_updated, render_system);
}

void render_system_update(const RenderSystem *const render_system)
{
}

void render_entity(Entity e, vec3 position, uint32_t texture_id)
{
}
