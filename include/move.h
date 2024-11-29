#ifndef MOVE_H
#define MOVE_H

#include "board.h"
void move(board_t* board, const char* from, const char* to);

_Bool pawn_can_move(board_t* board, const square_t* piece, const square_t* target);

#endif // MOVE_H
