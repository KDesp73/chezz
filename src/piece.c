#include "piece.h"
#include "square.h"

char piece_at(board_t* board, const square_t* square)
{
    return board->grid[PCOORDS(square)];
}

int piece_is(board_t* board, const square_t* square, int color)
{
    char piece = board->grid[PCOORDS(square)];
    if(piece == ' ') return 0;

    return (piece < 'a') == color;
}
