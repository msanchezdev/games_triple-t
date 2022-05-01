GAME_OBJS = $(wildcard src/*.cpp src/*.hpp)
ENGINE_OBJS = $(wildcard src/engine/*.cpp src/engine/*.hpp)
CC = g++

# -w suppresses all warnings
# -g enables debugging
COMPILER_FLAGS = -w -g
ENGINE_COMPILER_FLAGS = -shared -fPIC
GAME_COMPILER_FLAGS =

# link sdl2 library
LINKER_FLAGS = `sdl2-config --libs` -lSDL2_image
ENGINE_LINKER_FLAGS =
GAME_LINKER_FLAGS = -Wl,-rpath=. -Lbin -ltriton

BIN_DIR = bin
ENGINE_OBJ = libtriton.so
GAME_BIN = triple-t

game: $(BIN_DIR)/$(GAME_BIN)
$(BIN_DIR)/$(GAME_BIN): $(GAME_OBJS) $(BIN_DIR)/$(ENGINE_OBJ)
	mkdir -p $(BIN_DIR)
	$(CC) $< $(COMPILER_FLAGS) $(GAME_COMPILER_FLAGS) $(LINKER_FLAGS) $(GAME_LINKER_FLAGS) -o $@

engine: $(BIN_DIR)/$(ENGINE_OBJ)
$(BIN_DIR)/$(ENGINE_OBJ): $(ENGINE_OBJS)
	mkdir -p $(BIN_DIR);
	$(CC) $^ $(ENGINE_COMPILER_FLAGS) $(COMPILER_FLAGS) $(LINKER_FLAGS) $(ENGINE_LINKER_FLAGS) -o $@

	# for file in $(ENGINE_OBJS); do \
	# 	filename=`basename $(OBJ_DIR)/$${file%.cpp}.o`; \
	# 	$(CC) $$file $(COMPILER_FLAGS) $(ENGINE_COMPILER_FLAGS) -c -o $(OBJ_DIR)/$$filename; \
	# 	\
	# 	if [ $$? -ne 0 ]; then \
	# 		echo "Error compiling $$file"; \
	# 		exit 1; \
	# 	fi; \
	# done

assets: $(wildcard src/assets/**)
	mkdir -p $(BIN_DIR)/assets
	cp -r src/assets $(BIN_DIR)

run: game assets
	cd $(BIN_DIR) && ./$(GAME_BIN)
