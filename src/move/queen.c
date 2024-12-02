#include "extern/clib.h"
#include "move.h"
#include "piece.h"
#include <ctype.h>
#include <stdlib.h>

_Bool queen_can_move(board_t *board, const square_t *piece, const square_t *target)
{
    char _piece = board->grid[PCOORDS(piece)];
    int color = piece_color(_piece);

    // Validate that the piece is a queen
    if (tolower(_piece) != 'q') {
        // DEBU("Piece is not a queen");
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

    // Check if the move is valid for a rook or a bishop
    if (file_diff == 0 || rank_diff == 0) {
        // Attempt rook-like movement
        int step = (file_diff == 0) ? ((target->rank > piece->rank) ? 1 : -1) : ((target->file > piece->file) ? 1 : -1);

        // Rook-like vertical movement
        if (file_diff == 0) {
            for (int r = piece->rank + step; r != target->rank; r += step) {
                if (board->grid[r - 1][piece->file - 1] != EMPTY_SQUARE) {
                    // DEBU("Obstruction at rank: %d, file: %zu", r, piece->file);
                    board->error = ERROR_OBSTRUCTED_PATH;
                    return 0;
                }
            }
        }
        // Rook-like horizontal movement
        else {
            for (int f = piece->file + step; f != target->file; f += step) {
                if (board->grid[piece->rank - 1][f - 1] != EMPTY_SQUARE) {
                    // DEBU("Obstruction at rank: %zu, file: %d", piece->rank, f);
                    board->error = ERROR_OBSTRUCTED_PATH;
                    return 0;
                }
            }
        }
    } else if (abs(file_diff) == abs(rank_diff)) {
        // Attempt bishop-like diagonal movement
        int file_step = (target->file > piece->file) ? 1 : -1;
        int rank_step = (target->rank > piece->rank) ? 1 : -1;

        int current_file = piece->file + file_step;
        int current_rank = piece->rank + rank_step;

        while (current_file != target->file && current_rank != target->rank) {
            if (board->grid[current_rank - 1][current_file - 1] != EMPTY_SQUARE) {
                // DEBU("Obstruction at rank: %d, file: %d", current_rank, current_file);
                board->error = ERROR_OBSTRUCTED_PATH;
                return 0;
            }

            current_file += file_step;
            current_rank += rank_step;
        }
    } else {
        // Queen cannot move in any way other than rook or bishop rules
        board->error = ERROR_INVALID_MOVE;
        return 0;
    }

    // Check if the target square contains a piece of the same color
    char target_piece = board->grid[PCOORDS(target)];
    if (target_piece != EMPTY_SQUARE && piece_color(target_piece) == color) {
        // DEBU("Target square contains a piece of the same color");
        board->error = ERROR_FRIENDLY_PIECE;
        return 0;
    }

    board->error = 0;
    return 1; // Move is valid
}
