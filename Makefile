# Default to debug if no target is specified
BUILD_TYPE ?= debug

# Set CFLAGS, LDFLAGS, etc., based on the build type
ifeq ($(BUILD_TYPE), debug)
    CFLAGS += -g -O0  # Debug flags: no optimization, debug info
    LDFLAGS += -g  # Debug symbols for linking
else ifeq ($(BUILD_TYPE), release)
    CFLAGS += -O3  # Release flags: optimization
    LDFLAGS += -s  # Strip debugging symbols for smaller size
endif

# Default target is to build the ROM
all: test.z64
.PHONY: all

# Directories
BUILD_DIR = build
SRC_DIR = src
ASSET_DIR = assets

# Find all C source files recursively inside SRC_DIR and subfolders
SRC = $(shell find $(SRC_DIR) -name '*.c')

# Include directories for the compiler
INCLUDE_DIR = include
CFLAGS += -I$(INCLUDE_DIR)

# Include N64 and T3D libraries
include $(N64_INST)/include/n64.mk
include $(N64_INST)/include/t3d.mk

# Define asset paths for models and conversion
assets_models = $(wildcard $(ASSET_DIR)/models/*.glb)
assets_textures = $(wildcard $(ASSET_DIR)/textures/*.png)
assets_conv = $(addprefix filesystem/textures/,$(notdir $(assets_textures:%.png=%.sprite))) \
              $(addprefix filesystem/models/,$(notdir $(assets_models:%.glb=%.t3dm)))

# Convert PNGs to Sprites and put them in the filesystem
filesystem/textures/%.sprite: $(ASSET_DIR)/textures/%.png
	@mkdir -p $(dir $@)
	@echo "    [SPRITE] $@"
	$(N64_MKSPRITE) $(MKSPRITE_FLAGS) -o filesystem/textures "$<"

# Convert GLTF models to T3DM files and put them in the filesystem
filesystem/models/%.t3dm: $(ASSET_DIR)/models/%.glb
	@mkdir -p $(dir $@)
	@echo "    [T3D-MODEL] $@"
	$(T3D_GLTF_TO_3D) "$<" $@
	$(N64_BINDIR)/mkasset -c 2 -o filesystem/models $@

# Set object files (for .elf)
OBJS = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRC))

# Target: build the .elf file from object files
$(BUILD_DIR)/test.elf: $(OBJS)
$(BUILD_DIR)/test.dfs: $(assets_conv)

# Pattern rule to compile .c source files to .o object files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/test.dfs: $(assets_conv)

test.z64: N64_ROM_TITLE="Test"
test.z64: $(BUILD_DIR)/test.dfs

# Clean target: remove build files, .z64 file, and filesystem
clean:
	rm -rf $(BUILD_DIR) *.z64 filesystem
	@echo "Clean completed!"

.PHONY: clean

# Additional include for dependency files
-include $(wildcard $(BUILD_DIR)/*.d)

# Debug build target (optional to override default)
debug: BUILD_TYPE=debug
debug: all

# Release build target (optional to override default)
release: BUILD_TYPE=release
release: all
