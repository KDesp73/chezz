#include "board.h"
#include "extern/clib.h"
#include "move.h"
#include "piece.h"
#include <ctype.h>
#include <stdlib.h>

_Bool rook_can_move(board_t* board, const square_t* piece, const square_t* target)
{
    char _piece = board->grid[PCOORDS(piece)];
    int color = piece_color(_piece);

    // Validate that the piece is a rook
    if (tolower(_piece) != 'r') {
        // DEBU("Piece is not a rook");
        board->error = ERROR_INVALID_PIECE;
        return 0;
    }

    // Validate there is a piece at the source square
    if (color == PIECE_COLOR_NONE) {
        // DEBU("No piece found at: %s", piece->name);
        board->error = ERROR_EMPTY_SQUARE;
        return 0;
    }

    // Calculate file (horizontal) and rank (vertical) differences
    int file_diff = abs((int)piece->file - (int)target->file);
    int rank_diff = abs((int)target->rank - (int)piece->rank);

    // Rook moves must be either horizontal or vertical, not diagonal
    if (file_diff != 0 && rank_diff != 0) {
        // DEBU("Rook cannot move diagonally");
        board->error = ERROR_INVALID_MOVE;
        return 0;
    }

    // Check for obstructions along the path
    if (file_diff == 0) {
        // Moving vertically
        int step = (target->rank > piece->rank) ? 1 : -1;
        for (int r = piece->rank + step; r != target->rank; r += step) {
            if (board->grid[r - 1][piece->file - 1] != ' ') {
                // DEBU("Obstruction at rank: %d, file: %zu", r, piece->file);
                board->error = ERROR_OBSTRUCTED_PATH;
                return 0;
            }
        }
    } else if (rank_diff == 0) {
        // Moving horizontally
        int step = (target->file > piece->file) ? 1 : -1;
        for (int f = piece->file + step; f != target->file; f += step) {
            if (board->grid[piece->rank - 1][f - 1] != ' ') {
                // DEBU("Obstruction at rank: %zu, file: %d", piece->rank, f);
                board->error = ERROR_OBSTRUCTED_PATH;
                return 0;
            }
        }
    }

    // Check if the target square contains a piece of the same color
    char target_piece = board->grid[PCOORDS(target)];
    if (target_piece != PIECE_COLOR_NONE && piece_color(target_piece) == color) {
        // DEBU("Target square contains a piece of the same color");
        board->error = ERROR_FRIENDLY_PIECE;
        return 0;
    }

    board->error = 0;
    return 1; // Move is valid
}


