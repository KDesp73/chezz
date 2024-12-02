#ifndef BOARD_H
#define BOARD_H

#include "square.h"
#include <stdint.h>

#define BLACK_ROOK 'r'
#define BLACK_KNIGHT 'n'
#define BLACK_BISHOP 'b'
#define BLACK_KING 'k'
#define BLACK_QUEEN 'q'
#define BLACK_PAWN 'p'

#define WHITE_ROOK 'R'
#define WHITE_KNIGHT 'N'
#define WHITE_BISHOP 'B'
#define WHITE_KING 'K'
#define WHITE_QUEEN 'Q'
#define WHITE_PAWN 'P'

#define EMPTY_SQUARE ' '
#define BOARD_SIZE 8

#define STARTING_FEN "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"

typedef enum {
    ERROR_NONE = 0,
    ERROR_INVALID_MOVE,
    ERROR_EMPTY_SQUARE,
    ERROR_OBSTRUCTED_PATH,
    ERROR_INVALID_PIECE,
    ERROR_FRIENDLY_PIECE,
    ERROR_KINGS_TOUCHING,
    ERROR_INVALID_CASTLE,
    
} error_t;

static const char error_messages[][256] = {
    "OK",
    "Invalid move",
    "Empty square",
    "Obstructed path",
    "Invalid piece",
    "Attacking friendly piece",
    "Kings cannot touch",
    "Invalid castling"
};

typedef struct {
    char grid[8][8];
    _Bool turn;
    char enpassant_square[2 + 1]; // +1 for null terminator
    uint8_t castling_rights;
    size_t halfmove;
    size_t fullmove;
    error_t error;
    _Bool checkmate;
    uint8_t checks;
} board_t;

enum {
    CHECK_WHITE_KING = 0b01,
    CHECK_BLACK_KING = 0b10,
};

enum {
    CASTLE_WHITE_KINGSIDE = 0b0001,
    CASTLE_WHITE_QUEENSIDE = 0b0010,
    CASTLE_BLACK_KINGSIDE = 0b0100,
    CASTLE_BLACK_QUEENSIDE = 0b1000,
};

void board_init(board_t* board);
void board_init_fen(board_t* board, const char* fen);

typedef struct {
    _Bool errors;
    _Bool highlights;
    _Bool turn;
    _Bool checks;
    _Bool coords;
} print_config_t;

#define MINIMAL_CONFIG \
    (print_config_t) {.errors = 0,.highlights = 0,.turn = 0,.checks = 0, .coords = 1}
#define FULL_CONFIG \
    (print_config_t) {.errors = 1,.highlights = 1,.turn = 1,.checks = 1, .coords = 1}

void board_print(const board_t* board, print_config_t config, square_t* first, ...);

#define PRINT_PLAIN(board) \
    board_print(board, MINIMAL_CONFIG, NULL)
#define PRINT_FULL(board, first, ...) \
    board_print(board, FULL_CONFIG, first, ##__VA_ARGS__)


int has_castling_rights(const board_t* board, uint8_t castling_right);
void revoke_castling_rights(board_t* board, uint8_t castling_rights);

_Bool square_is_attacked(board_t* board, square_t* square, int color);
_Bool square_is_attacked_coords(board_t *board, int y, int x, int color);
_Bool square_is_attacked_fr(board_t *board, int rank, int file, int color);

square_t* find_king(board_t* board, int color);
#define FIND_KING_WHITE(board) \
    find_king(board, PIECE_COLOR_WHITE)
#define FIND_KING_BLACK(board) \
    find_king(board, PIECE_COLOR_BLACK)

_Bool king_in_check(const board_t* board, int color);
_Bool white_in_check(const board_t* board);
_Bool black_in_check(const board_t* board);
#define IN_CHECK(board, color) \
    square_is_attacked(board, find_king(board, color), color)

void update_castling_rights(board_t* board, const square_t* from);
void print_castling_rights(const board_t* board);

#endif // BOARD_H
