#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_rect.h>
#include <triton/engine.hpp>
#include <triton/components/sprite/sprite.hpp>
#include <triton/components/mouse_listener/mouse_listener.hpp>
#include "resources.hpp"

#define BOARD_SIZE 3
// char board[BOARD_SIZE][BOARD_SIZE];

TRT_GameObject* board = new TRT_GameObject();

#pragma region Base

void Game_Resources() {
    App_LoadImage(RES_IMG_BOARD);
    App_LoadImage(RES_IMG_CROSS);
    App_LoadImage(RES_IMG_CIRCLE);
}

void Game_Render(TRT_EventArgs<>* object) {
    // after 5 seconds hide the board
    if (SDL_GetTicks() > 5000) {
        board->GetComponent<TRT_Sprite>()->visible = false;
    }
}

void Board_OnMouseEnter(TRT_EventArgs<>* object, TRT_MouseEnterEvent* event) {
    info("mouse enter (%d, %d)", event->x, event->y);
}

void Board_OnMouseMove(TRT_EventArgs<TRT_MouseMoveEventData>* object, TRT_MouseMoveEvent* event) {
    info("mouse move (%d, %d)", event->x, event->y);
}

void Board_OnMouseLeave(TRT_EventArgs<>* object, TRT_MouseLeaveEvent* event) {
    info("mouse leave (%d, %d)", event->x, event->y);
}

void Board_OnMouseOver(TRT_EventArgs<>* object, TRT_MouseOverEvent* event) {
    info("mouse over (%d, %d)", event->x, event->y);
}

void Board_OnMouseDown(TRT_EventArgs<>* object, TRT_MouseDownEvent* event) {
    info("mouse down (%d, %d), button: %x, state: %x", event->x, event->y, event->button, event->state);
}

void Board_OnMouseUp(TRT_EventArgs<>* object, TRT_MouseUpEvent* event) {
    info("mouse up (%d, %d), button: %x, state: %x", event->x, event->y, event->button, event->state);
}

void Game_Init() {
    board->AddComponent(
        new TRT_Sprite(app.images[RES_IMG_BOARD]->surface, {
            .x = app.window_size.w / 2 - app.images[RES_IMG_BOARD]->surface->w / 2,
            .y = app.window_size.h / 2 - app.images[RES_IMG_BOARD]->surface->h / 2
        })
    );

    board->AddComponent(
        new TRT_MouseListener2D(TRT_MouseListenerHandlers {
            .on_mouse_enter = new TRT_MouseEnterHandler(Board_OnMouseEnter),
            .on_mouse_leave = new TRT_MouseLeaveHandler(Board_OnMouseLeave),
            .on_mouse_down = new TRT_MouseDownHandler(Board_OnMouseDown),
            .on_mouse_up = new TRT_MouseUpHandler(Board_OnMouseUp),
            .on_mouse_move = new TRT_MouseMoveHandler(Board_OnMouseMove),
            .on_mouse_over = new TRT_MouseOverHandler(Board_OnMouseOver)
        })
    );

    // app.events->Subscribe(board, TRT_EventType::EVENT_RENDER, new TRT_RenderEventHandler(Game_Render));

    /*
    sprite = new Sprite();
    sprite->Register();

    */

    // app.RegisterEventHandler(0, TRT_EventType::EVENT_RENDER, &Game_Render);

    // ImageResource* board_image = app.images[RES_IMG_BOARD];
    // board_rect = new SDL_Rect {

    //     .w = board_image->size->w,
    //     .h = board_image->size->h
    // };

    // symbol_size = new SDL_Size {
    //     .w = board_image->size->w / BOARD_SIZE,
    //     .h = board_image->size->h / BOARD_SIZE
    // };

    // board[1][1] = 'o';
}

void Game_Cleanup() {
    delete board;
}

#pragma endregion Base

#pragma region Graphics

// void Game_DrawSymbol(const char* image_key, Uint8 x, Uint8 y) {
//     ImageResource* image = app.images[image_key];

//     // draw the symbol centered in their cell
//     SDL_Rect rect {
//         .x = board_rect->x + (symbol_size->w * x) + (symbol_size->w / 2) - (image->size->w / 2),
//         .y = board_rect->y + (symbol_size->h * y) + (symbol_size->h / 2) - (image->size->h / 2),
//         .w = image->size->w,
//         .h = image->size->h
//     };
//     SDL_RenderCopy(app.renderer, image->texture, nullptr, &rect);
// }

// void Game_DrawSymbols() {
//     for (Uint8 x = 0; x < 3; x++) {
//         for (Uint8 y = 0; y < 3; y++) {
//             Uint8 occupied_by = board[x][y];
//             if (occupied_by == 'x') {
//                 Game_DrawSymbol(RES_IMG_CROSS, x, y);
//             } else if (occupied_by == 'o') {
//                 Game_DrawSymbol(RES_IMG_CIRCLE, x, y);
//             }
//         }
//     }
// }

#pragma endregion Graphics
