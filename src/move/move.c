#include "move.h"
#include "board.h"
#include "extern/clib.h"
#include "piece.h"
#include "square.h"
#include <string.h>

void move(board_t* board, square_t* from, square_t* to)
{
    char from_piece = board->grid[PCOORDS(from)];

    if(from_piece == ' ') return; // No piece to move
    
    board->grid[PCOORDS(from)] = ' ';
    board->grid[PCOORDS(to)] = from_piece;
}

_Bool piece_can_move(board_t* board, const square_t* piece, const square_t* target)
{
    assert(piece != NULL);
    assert(target != NULL);
    assert(board != NULL);

    switch (piece_at(board, piece)) {
        case 'K':
        case 'k':
            return king_can_move(board, piece, target);
        case 'Q':
        case 'q':
            return queen_can_move(board, piece, target);
        case 'R':
        case 'r':
            return rook_can_move(board, piece, target);
        case 'B':
        case 'b':
            return bishop_can_move(board, piece, target);
        case 'N':
        case 'n':
            return knight_can_move(board, piece, target);
        case 'P':
        case 'p':
            return pawn_can_move(board, piece, target);
        default:
            ERRO("Empty square");
            return 0;
    }
}
