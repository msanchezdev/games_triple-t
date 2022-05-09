#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_rect.h>
#include <triton/engine.hpp>
#include <triton/components/sprite/sprite.hpp>
#include <triton/components/mouse_listener/mouse_listener.hpp>
#include "resources.hpp"
#include "logic.hpp"

TRT_GameObject board("Board");
TRT_GameObject board_tile[BOARD_SIZE][BOARD_SIZE];

#pragma region Base

int main(int argc, char** argv) {
    app.Initialize();

    TRT_Point2D board_position {
        .x = app.resolution.width / 2 - app.images[RES_IMG_BOARD]->surface->w / 2,
        .y = app.resolution.height / 2 - app.images[RES_IMG_BOARD]->surface->h / 2
    };

    TRT_Sprite* board_sprite = new TRT_Sprite(app.images[RES_IMG_BOARD]->surface, &board_position);
    board.AddComponent(board_sprite);

    TRT_Size2D position_size {
        .width = app.images[RES_IMG_BOARD]->surface->w / BOARD_SIZE,
        .height = app.images[RES_IMG_BOARD]->surface->h / BOARD_SIZE
    };

    for (int x = 0; x < BOARD_SIZE; x++) {
        for (int y = 0; y < BOARD_SIZE; y++) {
            board_tile[x][y].name = "BoardTile-" + to_string(x) + "x" + to_string(y);

            TRT_Sprite* sprite = new TRT_Sprite(app.images[RES_IMG_CROSS]->surface, new TRT_Point2D {
                .x = board_position.x + (position_size.width * x),
                .y = board_position.y + (position_size.height * y)
            }, &position_size);
            sprite->SetOpacity(0.5)->SetVisible(false);

            TRT_MouseListener2D* mouse_listener = new TRT_MouseListener2D(
                sprite->rect,
                new BoardData {
                .x = x,
                .y = y
            });
            mouse_listener->Subscribe(
                TRT_MouseListener2D::EventType::MouseEnter,
                new TRT_MouseListener2D::MouseEnterEventHandler(BoardTile_MouseEnter, &board_tile[x][y])
            );
            mouse_listener->Subscribe(
                TRT_MouseListener2D::EventType::MouseLeave,
                new TRT_MouseListener2D::MouseLeaveEventHandler(BoardTile_MouseLeave, &board_tile[x][y])
            );
            mouse_listener->Subscribe(
                TRT_MouseListener2D::EventType::MouseDown,
                new TRT_MouseListener2D::MouseDownEventHandler(BoardTile_MouseDown, &board_tile[x][y])
            );

            board_tile[x][y].AddComponent(sprite);
            board_tile[x][y].AddComponent(mouse_listener);
            app.AddGameObject(&board_tile[x][y]);
        }
    }


    app.Start();
    app.Quit();

    return 0;
}

#pragma endregion Base
