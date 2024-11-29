#include "piece.h"
#include "board.h"
#include "square.h"

char piece_at(board_t* board, const square_t* square)
{
    return board->grid[PCOORDS(square)];
}

int piece_color(char piece)
{
    if(piece == ' ') return PIECE_COLOR_NONE;

    return (piece < 'a') == PIECE_COLOR_WHITE;
}

int piece_sees(board_t* board, const square_t* piece, const square_t* target)
{
    
}
