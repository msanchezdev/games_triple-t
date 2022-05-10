#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_rect.h>
#include <triton/engine.hpp>
#include <triton/components/sprite/sprite.hpp>
#include <triton/components/mouse_listener/mouse_listener.hpp>
#include "resources.hpp"
#include "logic.hpp"

using namespace triton;

GameObject board("Board");
GameObject board_tile[board_size][board_size];

#pragma region Base

int main(int argc, char** argv) {
    app.Initialize();

    Vector board_position(
        app.resolution.width / 2 - app.images[RES_IMG_BOARD]->surface->w / 2,
        app.resolution.height / 2 - app.images[RES_IMG_BOARD]->surface->h / 2
    );

    Sprite* board_sprite = new Sprite(app.images[RES_IMG_BOARD]->surface, &board_position);
    board.AddComponent(board_sprite);

    Size position_size(
        app.images[RES_IMG_BOARD]->surface->w / board_size,
        app.images[RES_IMG_BOARD]->surface->h / board_size
    );

    for (int x = 0; x < board_size; x++) {
        for (int y = 0; y < board_size; y++) {
            board_tile[x][y].name = "BoardTile-" + to_string(x) + "x" + to_string(y);

            Sprite* sprite = new Sprite(app.images[RES_IMG_CROSS]->surface, new Vector(
                board_position.x + (position_size.width * x),
                board_position.y + (position_size.height * y)
            ), &position_size);
            sprite->SetOpacity(0.5)->SetVisible(false);

            MouseListener* mouse_listener = new MouseListener(
                &sprite->rect,
                new BoardData {
                .x = x,
                .y = y
            });
            mouse_listener->Subscribe(
                MouseListener::EventType::MouseEnter,
                new MouseListener::MouseEnterEventHandler(BoardTile_MouseEnter, &board_tile[x][y])
            );
            mouse_listener->Subscribe(
                MouseListener::EventType::MouseLeave,
                new MouseListener::MouseLeaveEventHandler(BoardTile_MouseLeave, &board_tile[x][y])
            );
            mouse_listener->Subscribe(
                MouseListener::EventType::MouseDown,
                new MouseListener::MouseDownEventHandler(BoardTile_MouseDown, &board_tile[x][y])
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
