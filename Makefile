CC = clang
CFLAGS = -O2 -Wall -I.

EXAMPLES_DIR = example
BUILD_DIR = build
IMAGES_DIR = $(EXAMPLES_DIR)/images

# Automatically find all example C files
EXAMPLE_SRCS := $(wildcard $(EXAMPLES_DIR)/*.c)
EXAMPLE_BINS := $(patsubst $(EXAMPLES_DIR)/%.c,$(BUILD_DIR)/%,$(EXAMPLE_SRCS))

# Ensure directories exist
$(shell mkdir -p $(BUILD_DIR))
$(shell mkdir -p $(IMAGES_DIR))

# Default target: build and run all examples
all: $(EXAMPLE_BINS)
	@for bin in $(EXAMPLE_BINS); do \
		echo "Running $$bin..."; \
		./$$bin; \
	done

# Build each example individually (no linking with other files)
$(BUILD_DIR)/%: $(EXAMPLES_DIR)/%.c
	$(CC) $(CFLAGS) $< -o $@ -lm

clean:
	rm -rf $(BUILD_DIR)/*
	rm -rf $(IMAGES_DIR)/*
