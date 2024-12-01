#include "move.h"
#include "board.h"
#include "extern/clib.h"
#include "piece.h"
#include "square.h"
#include <ctype.h>
#include <stdlib.h>
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

square_t** valid_moves(board_t* board, const square_t* piece, size_t* count)
{
    if (piece_is_pinned(board, piece)) return NULL;

    size_t capacity = 10;
    *count = 0;
    square_t** moves = malloc(sizeof(square_t*) * capacity);

    for (int rank = 0; rank < BOARD_SIZE; rank++) {
        for (int file = 0; file < BOARD_SIZE; file++) {
            square_t* target = square_from_coords(rank, file);

            if (piece->rank == target->rank && piece->file == target->file) {
                continue;
            }

            if(tolower(piece_at(board, target)) == 'k') continue;

            if (piece_can_move(board, piece, target)) {
                if (*count == capacity) {
                    capacity *= 2;
                    moves = realloc(moves, sizeof(square_t*) * capacity);
                }
                moves[(*count)++] = target;
            }
        }
    }

    moves = realloc(moves, sizeof(square_t*) * *count);

    return moves;
}
