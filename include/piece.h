#ifndef PIECE_H
#define PIECE_H

#ifdef CHESS_DEVELOPMENT
#include "board.h"
#include "square.h"
#else
#include <chess/board.h>
#include <chess/square.h>
#endif // CHESS_DEVELOPMENT
#include <stdint.h>

#define PIECE_COLOR_BLACK 0
#define PIECE_COLOR_WHITE 1
#define PIECE_COLOR_NONE -1

char piece_at(const board_t* board, square_t square);
int piece_color(char piece);
#define PIECE_COLOR(board, square) \
    piece_color(piece_at(board, square))

_Bool piece_is_pinned(board_t* board, square_t piece);
_Bool piece_can_attack(board_t* board, square_t attacker, square_t target, _Bool strict);
_Bool piece_is_valid(char piece);

#endif // PIECE_H
