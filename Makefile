GAME_HEADERS = $(wildcard games/triple-t/*.hpp)
GAME_SOURCE = $(wildcard games/triple-t/*.cpp)
GAME_FILES = $(GAME_HEADERS) $(GAME_SOURCE)
CORE_HEADERS = $(wildcard src/triton/*.hpp) $(wildcard src/triton/types/*.hpp)
CORE_SOURCE = $(wildcard src/triton/*.cpp) $(wildcard src/triton/types/*.cpp)
CORE_FILES = $(CORE_HEADERS) $(CORE_SOURCE)
COMPONENTS_SOURCE = $(wildcard src/triton/components/**/*.cpp) $(wildcard src/triton/ui/**/*.cpp)
COMPONENTS_HEADERS = $(wildcard src/triton/components/**/*.hpp) $(wildcard src/triton/ui/**/*.hpp)
COMPONENTS_FILES = $(COMPONENTS_SOURCE) $(COMPONENTS_HEADERS)

BIN_DIR = bin/games/triple-t
LIB_NAME = triton-core
CORE_BIN = lib$(LIB_NAME).so
GAME_BIN = triple-t

CC = g++

# -w suppresses all warnings
# -g enables debugging
COMPILER_FLAGS = -w -O0 -g -std=c++17 -Isrc
CORE_COMPILER_FLAGS = -shared -fPIC
GAME_COMPILER_FLAGS =

# link sdl2 library
LINKER_FLAGS = `sdl2-config --libs` -lSDL2_image -lSDL2_ttf -lyaml-cpp
CORE_LINKER_FLAGS =
GAME_LINKER_FLAGS = -Wl,-rpath=. -L$(BIN_DIR) -l$(LIB_NAME)

game: $(BIN_DIR)/$(GAME_BIN)
$(BIN_DIR)/$(GAME_BIN): $(GAME_FILES) core
	mkdir -p $(BIN_DIR)
	$(CC) $(GAME_SOURCE) $(GAME_COMPILER_FLAGS) $(COMPILER_FLAGS) $(GAME_LINKER_FLAGS) $(LINKER_FLAGS) -o $@

core: $(BIN_DIR)/$(CORE_BIN)
$(BIN_DIR)/$(CORE_BIN): $(CORE_FILES) $(COMPONENTS_FILES)
	mkdir -p $(BIN_DIR)
	$(CC) $(CORE_SOURCE) $(COMPONENTS_SOURCE) $(CORE_COMPILER_FLAGS) $(COMPILER_FLAGS) $(CORE_LINKER_FLAGS) $(LINKER_FLAGS) -o $@

assets: games/triple-t/assets games/triple-t/triton.yaml
	mkdir -p $(BIN_DIR)
	cp -r $^ $(BIN_DIR)

run: game assets
	cd $(BIN_DIR) && ./$(GAME_BIN)

debug: game assets
	cd $(BIN_DIR) && gdb ./$(GAME_BIN)

debug-core: core
	cd $(BIN_DIR) && gdb ./$(CORE_BIN)

clean:
	rm -rf $(BIN_DIR)
