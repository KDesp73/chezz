#ifndef BOARD_H
#define BOARD_H

#include "square.h"
#define BLACK_ROOK 'r'
#define BLACK_KNIGHT 'n'
#define BLACK_BISHOP 'b'
#define BLACK_KING 'k'
#define BLACK_QUEEN 'q'
#define BLACK_PAWN 'p'
#define BLACK_BACK_RANK "rnbqkbnr"
#define BLACK_PAWN_RANK "pppppppp"

#define WHITE_ROOK 'R'
#define WHITE_KNIGHT 'N'
#define WHITE_BISHOP 'B'
#define WHITE_KING 'K'
#define WHITE_QUEEN 'Q'
#define WHITE_PAWN 'P'
#define WHITE_BACK_RANK "RNBQKBNR"
#define WHITE_PAWN_RANK "PPPPPPPP"

typedef struct {
    char grid[8][8];
} board_t;

void board_init(board_t* board);
void board_print(const board_t* board);
void board_print_highlight(const board_t* board, square_t** squares, size_t count);
void board_init_fen(board_t* board, const char* fen);


#endif // BOARD_H
