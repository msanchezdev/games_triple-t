OBJS = $(wildcard src/*.cpp)
CC = g++

# -w suppresses all warnings
# -g enables debugging
COMPILER_FLAGS = -w -g

# link sdl2
LINKER_FLAGS = `sdl2-config --libs` -lSDL2_image

OUTPUT_DIR = bin
OUTPUT_BIN = triple-t



build: $(OBJS)
	mkdir -p $(OUTPUT_DIR)
	$(CC) $(OBJS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OUTPUT_DIR)/$(OUTPUT_BIN)

assets: $(wildcard src/assets/**)
	mkdir -p $(OUTPUT_DIR)/assets
	cp -r src/assets $(OUTPUT_DIR)

run: build assets
	cd $(OUTPUT_DIR) && ./$(OUTPUT_BIN)
