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
    ERROR_INVALID_MOVE = 1,
    ERROR_EMPTY_SQUARE = 2,
    ERROR_OBSTRUCTED_PATH = 3,
    ERROR_INVALID_PIECE = 4,
    ERROR_FRIENDLY_PIECE = 5,
    ERROR_KINGS_TOUCHING = 6,
    ERROR_INVALID_CASTLE = 7,
    
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
void board_print(const board_t* board);
void board_print_highlight(const board_t* board, square_t** squares, size_t count);

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

_Bool white_in_check(const board_t* board);
_Bool black_in_check(const board_t* board);
#define IN_CHECK(board, color) \
    square_is_attacked(board, find_king(board, color), color)

#endif // BOARD_H
