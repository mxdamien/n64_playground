#include "assets.h"

static const char *model_filenames[] = {
    [ID_MODEL_TABLE] = "model.t3dm"};

static const char *texture_filenames[] = {};

static const char *font_filenames[] = {};

static const char *script_filenames[] = {};

static const char *sound_filenames[] = {};

const char *get_model_path(uint32_t model_id)
{
  if (model_id < sizeof(model_filenames) / sizeof(model_filenames[0]))
  {
    static char full_path[256];
    snprintf(full_path, sizeof(full_path), "rom:/%s/%s", ASSET_MODEL_PATH, model_filenames[model_id]);
    return full_path;
  }
  return ID_ASSET_INVALID;
}

const char *get_texture_path(uint32_t texture_id)
{
  if (texture_id < sizeof(texture_filenames) / sizeof(texture_filenames[0]))
  {
    static char full_path[256];
    snprintf(full_path, sizeof(full_path), "rom:/%s/%s", ASSET_TEXTURE_PATH, texture_filenames[texture_id]);
    return full_path;
  }
  return ID_ASSET_INVALID;
}

const char *get_fonth_path(uint32_t font_id)
{
  if (font_id < sizeof(font_filenames) / sizeof(font_filenames[0]))
  {
    static char full_path[256];
    snprintf(full_path, sizeof(full_path), "rom:/%s/%s", ASSET_FONT_PATH, font_filenames[font_id]);
    return full_path;
  }
  return ID_ASSET_INVALID;
}

const char *get_script_path(uint32_t script_id)
{
  if (script_id < sizeof(script_filenames) / sizeof(script_filenames[0]))
  {
    static char full_path[256];
    snprintf(full_path, sizeof(full_path), "rom:/%s/%s", ASSET_SCRIPT_PATH, script_filenames[script_id]);
    return full_path;
  }
  return ID_ASSET_INVALID;
}

const char *get_sound_path(uint32_t sound_id)
{
  if (sound_id < sizeof(sound_filenames) / sizeof(sound_filenames[0]))
  {
    static char full_path[256];
    snprintf(full_path, sizeof(full_path), "rom:/%s/%s", ASSET_SOUND_PATH, sound_filenames[sound_id]);
    return full_path;
  }
  return ID_ASSET_INVALID;
}