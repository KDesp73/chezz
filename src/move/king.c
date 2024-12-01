#include "extern/clib.h"
#include "move.h"
#include "piece.h"
#include <ctype.h>
#include "board.h"

_Bool kings_touching(const board_t* board, const square_t* target, int color)
{
    /*
        [ ] [ ] [ ]
     [ ]           [ ]
     [ ]     k     [ ]
     [ ]           [ ]
        [ ] [ ] [ ]
    */
    char other_king = (color == PIECE_COLOR_WHITE) ? 'k' : 'K';

    int offsets[8][2] = {
        {1, 1}, {1, 0}, {1, -1},
        {0, 1},         {0, -1},
        {-1, 1},{-1, 0},{-1, -1}
    };

    // Check if moving to the target square causes the kings to touch
    for (int i = 0; i < BOARD_SIZE; ++i) {
        int check_y = target->rank - 1 + offsets[i][0];
        int check_x = target->file - 1 + offsets[i][1];

        // Ensure the coordinates are within bounds
        if (check_y >= 0 && check_y < BOARD_SIZE && check_x >= 0 && check_x < BOARD_SIZE) {
            if (board->grid[check_y][check_x] == other_king) {
                return 1;
            }
        }
    }
    return 0;
}

_Bool king_can_move(board_t *board, const square_t *piece, const square_t *target)
{
    char _piece = board->grid[PCOORDS(piece)];
    int color = piece_color(_piece);

    // Validate that the piece is a king
    if (tolower(_piece) != 'k') {
        // DEBU("Piece is not a king");
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
    int file_diff = abs((int)target->file - (int)piece->file);
    int rank_diff = abs((int)target->rank - (int)piece->rank);

    // Kings move one square in any direction
    if (!(file_diff == 2 && rank_diff == 0) && !(file_diff <= 1 && rank_diff <= 1 && (file_diff + rank_diff > 0))) {
        // DEBU("Invalid king move");
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

    if(square_is_attacked(board, (square_t*)target, color)) {
        // DEBU("King would be in check");
        board->error = ERROR_INVALID_MOVE;
        return 0;
    }

    // Check if moving to the target square leads to the kings touching
    if(kings_touching(board, target, color)){
        // DEBU("Kings cannot touch");
        board->error = ERROR_KINGS_TOUCHING;
        return 0;
    }

    // Check if the other king is present around the king

    // Castling logic
    if (file_diff == 2 && rank_diff == 0) {
        // Castling is a horizontal move of 2 squares
        if (color == PIECE_COLOR_WHITE) {
            // White king castling
            if (target->file > piece->file) {
                // Kingside castling
                if (!(board->castling_rights & CASTLE_WHITE_KINGSIDE)) {
                    // DEBU("White kingside castling not allowed");
                    board->error = ERROR_INVALID_CASTLE;
                    return 0;
                }
                // Ensure squares between king and rook are empty
                if (board->grid[piece->rank - 1][5] != EMPTY_SQUARE || board->grid[piece->rank - 1][6] != EMPTY_SQUARE) {
                    // DEBU("Path to white kingside castling is not clear");
                    board->error = ERROR_OBSTRUCTED_PATH;
                    return 0;
                }
            } else {
                // Queenside castling
                if (!(board->castling_rights & CASTLE_WHITE_QUEENSIDE)) {
                    // DEBU("White queenside castling not allowed");
                    board->error = ERROR_INVALID_CASTLE;
                    return 0;
                }
                // Ensure squares between king and rook are empty
                if (board->grid[piece->rank - 1][1] != EMPTY_SQUARE || board->grid[piece->rank - 1][2] != EMPTY_SQUARE || board->grid[piece->rank - 1][3] != EMPTY_SQUARE) {
                    // DEBU("Path to white queenside castling is not clear");
                    board->error = ERROR_OBSTRUCTED_PATH;
                    return 0;
                }
            }
        } else {
            // Black king castling
            if (target->file > piece->file) {
                // Kingside castling
                if (!(board->castling_rights & CASTLE_BLACK_KINGSIDE)) {
                    // DEBU("Black kingside castling not allowed");
                    board->error = ERROR_INVALID_CASTLE;
                    return 0;
                }
                // Ensure squares between king and rook are empty
                if (board->grid[piece->rank - 1][5] != EMPTY_SQUARE || board->grid[piece->rank - 1][6] != EMPTY_SQUARE) {
                    // DEBU("Path to black kingside castling is not clear");
                    board->error = ERROR_OBSTRUCTED_PATH;
                    return 0;
                }
            } else {
                // Queenside castling
                if (!(board->castling_rights & CASTLE_BLACK_QUEENSIDE)) {
                    // DEBU("Black queenside castling not allowed");
                    board->error = ERROR_INVALID_CASTLE;
                    return 0;
                }
                // Ensure squares between king and rook are empty
                if (board->grid[piece->rank - 1][1] != EMPTY_SQUARE || board->grid[piece->rank - 1][2] != EMPTY_SQUARE || board->grid[piece->rank - 1][3] != EMPTY_SQUARE) {
                    // DEBU("Path to black queenside castling is not clear");
                    board->error = ERROR_OBSTRUCTED_PATH;
                    return 0;
                }
            }
        }


        // Ensure the king and rook do not pass through or land on attacked squares
        // Determine castling direction (kingside or queenside)
        int castle_direction = (target->x > piece->x) ? 1 : -1;

        if (square_is_attacked_coords(board, piece->y, piece->x + castle_direction, color) ||
                square_is_attacked_coords(board, target->y, target->x, color)) {
            // DEBU("Cannot castle through or into check");
            board->error = ERROR_INVALID_CASTLE;
            return 0;
        }

        // Check if the target square (final destination) leads to a king touching scenario
        if (kings_touching(board, target, color)) {
            // DEBU("Kings cannot touch");
            board->error = ERROR_KINGS_TOUCHING;
            return 0;
        }
    }

    return 1; // Move is valid
}
