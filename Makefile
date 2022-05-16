GAME_HEADERS = $(wildcard games/triple-t/*.hpp)
GAME_SOURCE = $(wildcard games/triple-t/*.cpp)
GAME_FILES = $(GAME_HEADERS) $(GAME_SOURCE)
CORE_HEADERS = $(wildcard src/triton/*.hpp) $(wildcard src/triton/types/*.hpp)
CORE_SOURCE = $(wildcard src/triton/*.cpp) $(wildcard src/triton/types/*.cpp)
CORE_FILES = $(CORE_HEADERS) $(CORE_SOURCE)
COMPONENTS_SOURCE = $(wildcard src/triton/components/**/*.cpp) $(wildcard src/triton/ui/**/*.cpp)
COMPONENTS_HEADERS = $(wildcard src/triton/components/**/*.hpp) $(wildcard src/triton/ui/**/*.hpp)
COMPONENTS_FILES = $(COMPONENTS_SOURCE) $(COMPONENTS_HEADERS)

BIN_DIR = bin
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

games = triple-t basketball

# join bin/ to every file in games
all: $(games)

$(games): %: $(wildcard games/%/*.cpp) core
# strip the path and extension from the file name
	@game_binary=`basename $(patsubst bin/%,%,$@)`;\
\
	mkdir -p $(BIN_DIR)/$$game_binary; \
	echo "Compiling game $$game_binary"; \
	$(CC) $(wildcard games/$(patsubst bin/%,%,$@)/*.cpp) $(GAME_COMPILER_FLAGS) $(COMPILER_FLAGS) $(GAME_LINKER_FLAGS) $(LINKER_FLAGS) -o $(BIN_DIR)/$$game_binary/$$game_binary; \
\
	echo "Copying core files to $(BIN_DIR)/$$game_binary"; \
	cp $(BIN_DIR)/$(CORE_BIN) $(BIN_DIR)/$$game_binary/; \
\
	echo "Copying assets to $(BIN_DIR)/$$game_binary/assets"; \
	cp -r games/$$game_binary/assets $(BIN_DIR)/$$game_binary/; \
\
	echo "Copying triton.yaml to $(BIN_DIR)/$$game_binary/triton.yaml"; \
	cp games/$$game_binary/triton.yaml $(BIN_DIR)/$$game_binary/; \
	echo;


core: $(BIN_DIR)/$(CORE_BIN)
$(BIN_DIR)/$(CORE_BIN): $(CORE_FILES) $(COMPONENTS_FILES)
	mkdir -p $(BIN_DIR)
	$(CC) $(CORE_SOURCE) $(COMPONENTS_SOURCE) $(CORE_COMPILER_FLAGS) $(COMPILER_FLAGS) $(CORE_LINKER_FLAGS) $(LINKER_FLAGS) -o $@

$(patsubst %,run-%,$(games)): run-%: %
	cd $(BIN_DIR)/$< && ./$<

$(patsubst %,debug-%,$(games)): debug-%: %
	cd $(BIN_DIR)/$< && gdb ./$<

clean:
	rm -rf $(BIN_DIR)
