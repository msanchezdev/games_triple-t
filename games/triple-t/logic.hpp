#include <triton/engine.hpp>
#include <triton/components/sprite/sprite.hpp>
#include <triton/components/mouse_listener/mouse_listener.hpp>
const int board_size = 3;

using namespace triton;

bool game_running = true;
char player = 'X';
char board_tile_value[board_size][board_size] = {
    {' ', ' ', ' '},
    {' ', ' ', ' '},
    {' ', ' ', ' '}
};

struct BoardData {
    int x;
    int y;
};

// tic tac toe win check logic
void WinCheck() {
    // check rows
    for (int i = 0; i < board_size; i++) {
        if (board_tile_value[i][0] == board_tile_value[i][1] && board_tile_value[i][1] == board_tile_value[i][2]) {
            if (board_tile_value[i][0] != ' ') {
                info("%c wins!\n", board_tile_value[i][0]);
                game_running = false;
                return;
            }
        }
    }

    // check columns
    for (int i = 0; i < board_size; i++) {
        if (board_tile_value[0][i] == board_tile_value[1][i] && board_tile_value[1][i] == board_tile_value[2][i]) {
            if (board_tile_value[0][i] != ' ') {
                info("%c wins!\n", board_tile_value[0][i]);
                game_running = false;
                return;
            }
        }
    }

    // check diagonals
    if (board_tile_value[0][0] == board_tile_value[1][1] && board_tile_value[1][1] == board_tile_value[2][2]) {
        if (board_tile_value[0][0] != ' ') {
            info("%c wins!\n", board_tile_value[0][0]);
            game_running = false;
            return;
        }
    }

    if (board_tile_value[0][2] == board_tile_value[1][1] && board_tile_value[1][1] == board_tile_value[2][0]) {
        if (board_tile_value[0][2] != ' ') {
            info("%c wins!\n", board_tile_value[0][2]);
            game_running = false;
            return;
        }
    }
}

void BoardTile_MouseEnter(EventArgs<MouseListener, GameObject, MouseListener::MouseEnterEvent>* event) {
    if (!game_running) return;

    BoardData* data = (BoardData*)event->sender->data;
    Sprite* sprite = event->reference->GetComponent<Sprite>();
    char occoupied_by = board_tile_value[data->x][data->y];
    info("Mouse Enter: %d, %d\n", data->x, data->y);

    if (occoupied_by == ' ') {
        sprite->SetImage(app.images[player == 'X' ? RES_IMG_CROSS : RES_IMG_CIRCLE]->surface);
        sprite->SetVisible(true);
    }
}

void BoardTile_MouseLeave(EventArgs<MouseListener, GameObject, MouseListener::MouseLeaveEvent>* event) {
    if (!game_running) return;

    BoardData* data = (BoardData*)event->sender->data;
    Sprite* sprite = event->reference->GetComponent<Sprite>();
    char occoupied_by = board_tile_value[data->x][data->y];
    info("Mouse Leave: %d, %d\n", data->x, data->y);

    if (occoupied_by == ' ') {

        sprite->SetVisible(false);
    }
}

void BoardTile_MouseDown(EventArgs<MouseListener, GameObject, MouseListener::MouseDownEvent>* event) {
    if (!game_running) return;

    BoardData* data = (BoardData*)event->sender->data;
    Sprite* sprite = event->reference->GetComponent<Sprite>();
    char* occoupied_by = &board_tile_value[data->x][data->y];
    info("Mouse Down: %d, %d\n", data->x, data->y);

    if (*occoupied_by == ' ') {
        *occoupied_by = player;
        sprite->SetImage(app.images[player == 'X' ? RES_IMG_CROSS : RES_IMG_CIRCLE]->surface);
        sprite->SetOpacity(1);
        player = player == 'X' ? 'O' : 'X';
    }

    WinCheck();
}
