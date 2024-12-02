#ifndef PIECE_H
#define PIECE_H

#include "board.h"
#include "square.h"

#define PIECE_COLOR_BLACK 0
#define PIECE_COLOR_WHITE 1
#define PIECE_COLOR_NONE -1

char piece_at(const board_t* board, const square_t* square);
int piece_color(char piece);
_Bool piece_is_pinned(board_t* board, const square_t* square);
_Bool piece_can_attack(board_t* board, const square_t* attacker, const square_t* target);

#endif // PIECE_H
