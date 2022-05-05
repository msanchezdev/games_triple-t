#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_rect.h>
#include <triton/engine.hpp>
#include <triton/components/sprite/sprite.hpp>
#include <triton/components/mouse_listener/mouse_listener.hpp>
#include "resources.hpp"
#include "logic.hpp"


TRT_GameObject board;
TRT_GameObject board_tile[BOARD_SIZE][BOARD_SIZE];


#pragma region Base

void Game_Resources() {
    App_LoadImage(RES_IMG_BOARD);
    App_LoadImage(RES_IMG_CROSS);
    App_LoadImage(RES_IMG_CIRCLE);
}

void Game_Init() {
    TRT_Point2D* board_position = new TRT_Point2D {
        .x = app.window_size.w / 2 - app.images[RES_IMG_BOARD]->surface->w / 2,
        .y = app.window_size.h / 2 - app.images[RES_IMG_BOARD]->surface->h / 2
    };

    board.AddComponent(
        new TRT_Sprite(app.images[RES_IMG_BOARD]->surface, board_position)
    );

    app.events->Subscribe(&board, TRT_EventType::EVENT_RENDER, new TRT_RenderEventHandler(WinCheck));

    TRT_Size2D* position_size = new TRT_Size2D {
        .width = app.images[RES_IMG_BOARD]->surface->w / BOARD_SIZE,
        .height = app.images[RES_IMG_BOARD]->surface->h / BOARD_SIZE
    };

    for (int x = 0; x < BOARD_SIZE; x++) {
        for (int y = 0; y < BOARD_SIZE; y++) {
            board_tile[x][y].AddComponent(
                (new TRT_Sprite(app.images[RES_IMG_CROSS]->surface, new TRT_Point2D {
                    .x = board_position->x + (position_size->width * x),
                    .y = board_position->y + (position_size->height * y)
                }, position_size))->SetVisible(false)->SetOpacity(0.5)
            );

            board_tile[x][y].AddComponent(
                new TRT_MouseListener2D(TRT_MouseListenerHandlers {
                    .on_mouse_enter = new TRT_MouseEnterHandler(BoardTile_MouseEnter),
                    .on_mouse_leave = new TRT_MouseLeaveHandler(BoardTile_MouseLeave),
                    .on_mouse_down = new TRT_MouseDownHandler(BoardTile_MouseDown)
                }, new BoardData {
                    .x = x,
                    .y = y
                })
            );
        }
    }
}

void Game_Cleanup() {
}

#pragma endregion Base
