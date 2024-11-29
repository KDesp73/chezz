#ifndef PIECE_H
#define PIECE_H

#include "board.h"

#define PIECE_COLOR_BLACK 0
#define PIECE_COLOR_WHITE 1
#define PIECE_COLOR_NONE -1

char piece_at(board_t* board, const square_t* square);
int piece_color(char piece);

#endif // PIECE_H
