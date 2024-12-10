#include "board.h"
#include "extern/clib.h"
#include "move.h"
#include "piece.h"
#include <ctype.h>
#include <stdlib.h>

#undef DEBU
#define DEBU(format, ...)

_Bool bishop_can_move(board_t *board, square_t piece, square_t target)
{
    char _piece = board->grid[COORDS(piece)];
    int color = piece_color(_piece);

    // Validate that the piece is a bishop
    if (tolower(_piece) != 'b') {
        DEBU("Piece is not a bishop");
        board->state.error = ERROR_INVALID_PIECE;
        return 0;
    }

    // Validate there is a piece at the source square
    if (color == PIECE_COLOR_NONE) {
        DEBU("No piece found at: %s", piece->name);
        board->state.error = ERROR_EMPTY_SQUARE;
        return 0;
    }

    // Calculate file (horizontal) and rank (vertical) differences
    int file_diff = (int)target.file - (int)piece.file;
    int rank_diff = (int)target.rank - (int)piece.rank;

    // Bishops must move diagonally (absolute differences must be equal)
    if (abs(file_diff) != abs(rank_diff)) {
        DEBU("Invalid diagonal movement");
        board->state.error = ERROR_INVALID_MOVE;
        return 0;
    }

    // Determine direction of movement
    int file_step = (file_diff > 0) ? 1 : -1;
    int rank_step = (rank_diff > 0) ? 1 : -1;

    // Check for obstructions along the diagonal path
    int current_file = piece.file + file_step;
    int current_rank = piece.rank + rank_step;

    while (current_file != target.file && current_rank != target.rank) {
        if (board->grid[current_rank - 1][current_file - 1] != EMPTY_SQUARE) {
            DEBU("Obstruction at rank: %d, file: %d", current_rank, current_file);
            board->state.error = ERROR_OBSTRUCTED_PATH;
            return 0;
        }

        current_file += file_step;
        current_rank += rank_step;
    }

    // Check if the target square contains a piece of the same color
    char target_piece = board->grid[COORDS(target)];
    if (target_piece != EMPTY_SQUARE && piece_color(target_piece) == color) {
        DEBU("Target square contains a piece of the same color");
        board->state.error = ERROR_FRIENDLY_PIECE;
        return 0;
    }

    board->state.error = 0;
    return 1; // Move is valid
}
