CC = clang
CFLAGS = -O2 -Wall -I.

EXAMPLE_SRC = example/example.c
EXAMPLE_BIN = build/example
IMAGES_DIR = example/images

# Default target: build and run
all: run

# Ensure the build directory exists and compile
$(EXAMPLE_BIN): $(EXAMPLE_SRC) apricot.c
	mkdir -p build
	mkdir -p $(IMAGES_DIR)
	$(CC) $(CFLAGS) $(EXAMPLE_SRC) apricot.c -o $(EXAMPLE_BIN)

# Run the built program
run: $(EXAMPLE_BIN)
	./$(EXAMPLE_BIN)

clean:
	rm -f $(EXAMPLE_BIN)
	rm -rf $(IMAGES_DIR)
	rm -rf build
