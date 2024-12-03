#include "extern/clib.h"
#include "move.h"
#include "piece.h"
#include <ctype.h>

#undef DEBU
#define DEBU(format, ...)

_Bool knight_can_move(board_t *board, square_t piece, square_t target)
{
    char _piece = board->grid[COORDS(piece)];
    int color = piece_color(_piece);

    // Validate that the piece is a knight
    if (tolower(_piece) != 'n') {
        DEBU("Piece is not a knight");
        board->error = ERROR_INVALID_PIECE;
        return 0;
    }

    // Validate there is a piece at the source square
    if (color == PIECE_COLOR_NONE) {
        DEBU("No piece found at: %s", piece.name);
        board->error = ERROR_EMPTY_SQUARE;
        return 0;
    }

    // Calculate file (horizontal) and rank (vertical) differences
    int file_diff = abs((int)target.file - (int)piece.file);
    int rank_diff = abs((int)target.rank - (int)piece.rank);

    // Knights move in an "L" shape: one rank and two files, or two ranks and one file
    if (!((file_diff == 1 && rank_diff == 2) || (file_diff == 2 && rank_diff == 1))) {
        DEBU("Invalid knight move");
        board->error = ERROR_INVALID_MOVE;
        return 0;
    }

    // Check if the target square contains a piece of the same color
    char target_piece = board->grid[COORDS(target)];
    if (target_piece != EMPTY_SQUARE && piece_color(target_piece) == color) {
        DEBU("Target square contains a piece of the same color");
        board->error = ERROR_FRIENDLY_PIECE;
        return 0;
    }

    board->error = 0;
    return 1; // Move is valid
}
