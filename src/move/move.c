#include "move.h"
#include "board.h"
#include "square.h"

void move(board_t* board, const char* from, const char* to)
{
    square_t* from_square = square_from_name(from);
    square_t* to_square = square_from_name(to);

    char from_piece = board->grid[PCOORDS(from_square)];

    if(from_piece == ' ') return; // No piece to move
    
    board->grid[PCOORDS(from_square)] = ' ';
    board->grid[PCOORDS(to_square)] = from_piece;
}

