#ifndef ASSETS_H
#define ASSETS_H

#include <stdint.h>
#include <stdio.h>

#define ID_MODEL_TABLE 0

#define ASSET_MODEL_PATH "models"
#define ASSET_FONT_PATH "fonts"
#define ASSET_SCRIPT_PATH "scripts"
#define ASSET_SOUND_PATH "sounds"
#define ASSET_TEXTURE_PATH "textures"

#define ID_ASSET_INVALID "ASSET_ID_INVALID"

const char *GetModelPath(uint32_t model_id);
const char *GetTexturePath(uint32_t texture_id);
const char *GetFontPath(uint32_t font_id);
const char *GetScriptPath(uint32_t script_id);
const char *GetSoundPath(uint32_t sound_id);

#endif
