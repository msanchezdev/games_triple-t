GAME_HEADERS = $(wildcard games/triple-t/*.hpp)
GAME_SOURCE = $(wildcard games/triple-t/*.cpp)
GAME_FILES = $(GAME_HEADERS) $(GAME_SOURCE)
ENGINE_HEADERS = $(wildcard src/triton/*.hpp)
ENGINE_SOURCE = $(wildcard src/triton/*.cpp)
ENGINE_FILES = $(ENGINE_HEADERS) $(ENGINE_SOURCE)
COMPONENTS_SOURCE = $(wildcard src/triton/components/**/*.cpp)
COMPONENTS_HEADERS = $(wildcard src/triton/components/**/*.hpp)
COMPONENTS_FILES = $(COMPONENTS_SOURCE) $(COMPONENTS_HEADERS)

BIN_DIR = bin/games/triple-t
ENGINE_BIN = libtriton.so
GAME_BIN = triple-t

CC = g++

# -w suppresses all warnings
# -g enables debugging
COMPILER_FLAGS = -w -g -std=c++17 -Isrc
ENGINE_COMPILER_FLAGS = -shared -fPIC
GAME_COMPILER_FLAGS =

# link sdl2 library
LINKER_FLAGS = `sdl2-config --libs` -lSDL2_image
ENGINE_LINKER_FLAGS =
GAME_LINKER_FLAGS = -Wl,-rpath=. -L$(BIN_DIR) -ltriton

game: $(BIN_DIR)/$(GAME_BIN)
$(BIN_DIR)/$(GAME_BIN): $(GAME_FILES) engine
	mkdir -p $(BIN_DIR)
	$(CC) $(GAME_SOURCE) $(COMPILER_FLAGS) $(GAME_COMPILER_FLAGS) $(LINKER_FLAGS) $(GAME_LINKER_FLAGS) -o $@

engine: $(BIN_DIR)/$(ENGINE_BIN)
$(BIN_DIR)/$(ENGINE_BIN): $(ENGINE_FILES) $(COMPONENTS_FILES)
	mkdir -p $(BIN_DIR)
	$(CC) $(ENGINE_SOURCE) $(COMPONENTS_SOURCE) $(ENGINE_COMPILER_FLAGS) $(COMPILER_FLAGS) $(LINKER_FLAGS) $(ENGINE_LINKER_FLAGS) -o $@

assets: $(wildcard games/triple-t/assets/**)
	mkdir -p $(BIN_DIR)/assets
	cp -r $^ $(BIN_DIR)/assets

run: game assets
	cd $(BIN_DIR) && ./$(GAME_BIN)

debug: game assets
	cd $(BIN_DIR) && gdb ./$(GAME_BIN)

debug-engine: engine
	cd $(BIN_DIR) && gdb ./$(ENGINE_BIN)

clean:
	rm -rf $(BIN_DIR)
