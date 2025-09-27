# Default to release if no target is specified
BUILD_TYPE ?= release

# Set CFLAGS, LDFLAGS, etc., based on the build type
ifeq ($(BUILD_TYPE), debug)
    CFLAGS += -g -O0  # Debug flags: no optimization, debug info
    LDFLAGS += -g  # Debug symbols for linking
else ifeq ($(BUILD_TYPE), release)
    CFLAGS += -O3  # Release flags: optimization
    LDFLAGS += -s  # Strip debugging symbols for smaller size
endif

all: test.z64
.PHONY: all

BUILD_DIR = build
SRC_DIR = src
# Find all C source files recursively inside SRC_DIR and subfolders
SRC = $(shell find $(SRC_DIR) -name '*.c')
INCLUDE_DIR = include

# Add include directory to CFLAGS so the compiler can find header files
CFLAGS += -I$(INCLUDE_DIR)

include $(N64_INST)/include/n64.mk

# Set object files
OBJS = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRC))

# ROM Title
test.z64: N64_ROM_TITLE = "Test"

# Target: build the .elf file from object files
$(BUILD_DIR)/test.elf: $(OBJS)

# Pattern rule to compile .c source files to .o object files,
# creating needed subdirectories in build directory
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean target: remove build files and .z64 file
clean:
	rm -rf $(BUILD_DIR) *.z64
.PHONY: clean

# Additional include for dependency files
-include $(wildcard $(BUILD_DIR)/*.d)

# Debug build target (optional to override default)
debug: BUILD_TYPE=debug
debug: all

# Release build target (optional to override default)
release: BUILD_TYPE=release
release: all
