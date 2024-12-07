#ifndef UI_H
#define UI_H

#include "board.h"
#include "square.h"

typedef struct {
    _Bool castling;
    _Bool checks;
    _Bool coords;
    _Bool enpassant;
    _Bool errors;
    _Bool fullmove;
    _Bool halfmove;
    _Bool hash;
    _Bool highlights;
    _Bool perspective;
    _Bool turn;
} ui_config_t;

#define MINIMAL_CONFIG \
    (ui_config_t) {.errors = 0,.highlights = 0,.turn = 0,.checks = 0, .castling = 0, .enpassant = 0, .halfmove = 0, .fullmove = 0, .hash = 0, .coords = 1, .perspective = 1}
#define FULL_CONFIG \
    (ui_config_t) {.errors = 1,.highlights = 1,.turn = 1,.checks = 1, .castling = 1, .enpassant = 1, .halfmove = 1, .fullmove = 1, .hash = 1, .coords = 1, .perspective = 1}

void tui_board_print_squares(const board_t* board, ui_config_t config, square_t** squares, size_t count);
void tui_board_print(const board_t* board, ui_config_t config, square_t* first, ...);

#define PRINT_PLAIN(board) \
    tui_board_print(board, MINIMAL_CONFIG, NULL)
#define PRINT_FULL(board, first, ...) \
    tui_board_print(board, FULL_CONFIG, first, ##__VA_ARGS__)

void tui_print_castling_rights(const board_t* board);
void tui_run(const char* fen, ui_config_t config);


#endif // UI_H
