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
    ERROR_INVALID_MOVE,
    ERROR_EMPTY_SQUARE,
    ERROR_OBSTRUCTED_PATH,
    ERROR_INVALID_PIECE,
    ERROR_FRIENDLY_PIECE,
    ERROR_KINGS_TOUCHING,
    ERROR_INVALID_CASTLE,
    
} error_t;

typedef struct {
    char grid[8][8];
    _Bool turn;
    char enpassant_square[2 + 1]; // +1 for null terminator
    uint8_t castling_rights;
    size_t halfmove;
    size_t fullmove;
    error_t error;
} board_t;


enum {
    CASTLE_WHITE_KINGSIDE = 0b0001,
    CASTLE_WHITE_QUEENSIDE = 0b0010,
    CASTLE_BLACK_KINGSIDE = 0b0100,
    CASTLE_BLACK_QUEENSIDE = 0b1000,
};

void board_init(board_t* board);
void board_print(const board_t* board);
void board_print_highlight(const board_t* board, square_t** squares, size_t count);
void board_init_fen(board_t* board, const char* fen);

int has_castling_rights(const board_t* board, uint8_t castling_right);
void revoke_castling_rights(board_t* board, uint8_t castling_rights);

_Bool square_is_attacked(board_t* board, square_t* square, int color);
_Bool square_is_attacked_coords(board_t *board, int y, int x, int color);
_Bool square_is_attacked_fr(board_t *board, int rank, int file, int color);

#endif // BOARD_H
