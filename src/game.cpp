#include <SDL2/SDL_render.h>
#include <SDL2/SDL_rect.h>
#include "engine/engine.hpp"
#include "resources.hpp"

#define BOARD_SIZE 3
Uint8 board[BOARD_SIZE][BOARD_SIZE];

SDL_Rect* board_rect = NULL;
SDL_Size* symbol_size = NULL;

void Game_DrawBoard();
void Game_DrawSymbols();

#pragma region Base

void Game_Init() {
    ImageResource* board_image = app.images[RES_IMG_BOARD];
    board_rect = new SDL_Rect {
        .x = app.window_size.w / 2 - board_image->size->w / 2,
        .y = app.window_size.h / 2 - board_image->size->h / 2,
        .w = board_image->size->w,
        .h = board_image->size->h
    };

    symbol_size = new SDL_Size {
        .w = board_image->size->w / BOARD_SIZE,
        .h = board_image->size->h / BOARD_SIZE
    };

    board[0][0] = 'x';
    board[0][1] = 'o';
    board[2][0] = 'x';
}

void Game_Resources() {
    App_LoadImage(RES_IMG_BOARD);
    App_LoadImage(RES_IMG_CROSS);
    App_LoadImage(RES_IMG_CIRCLE);
}

void Game_Cleanup() {
    delete board_rect;
    delete symbol_size;
}

void Game_Render() {
    Game_DrawBoard();
    Game_DrawSymbols();
}

#pragma endregion Base

#pragma region Graphics

void Game_DrawBoard() {
    ImageResource* board_image = app.images[RES_IMG_BOARD];
    SDL_RenderCopy(app.renderer, board_image->texture, NULL, board_rect);
}

void Game_DrawSymbol(const char* image_key, Uint8 x, Uint8 y) {
    ImageResource* image = app.images[image_key];

    // draw the symbol centered in their cell
    SDL_Rect rect {
        .x = board_rect->x + (symbol_size->w * x) + (symbol_size->w / 2) - (image->size->w / 2),
        .y = board_rect->y + (symbol_size->h * y) + (symbol_size->h / 2) - (image->size->h / 2),
        .w = image->size->w,
        .h = image->size->h
    };
    SDL_RenderCopy(app.renderer, image->texture, NULL, &rect);
}

void Game_DrawSymbols() {
    for (Uint8 x = 0; x < 3; x++) {
        for (Uint8 y = 0; y < 3; y++) {
            Uint8 occupied_by = board[x][y];
            if (occupied_by == 'x') {
                Game_DrawSymbol(RES_IMG_CROSS, x, y);
            } else if (occupied_by == 'o') {
                Game_DrawSymbol(RES_IMG_CIRCLE, x, y);
            }
        }
    }
}

#pragma endregion Graphics
