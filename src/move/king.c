#include "extern/clib.h"
#include "move.h"
#include "piece.h"
#include <ctype.h>
#include "board.h"

#undef DEBU
#define DEBU(format, ...)

_Bool kings_touching(const board_t* board, square_t target, int color)
{
    char other_king = (color == PIECE_COLOR_WHITE) ? 'k' : 'K';
    /*
        [ ] [ ] [ ]
     [ ]           [ ]
     [ ]     k     [ ]
     [ ]           [ ]
        [ ] [ ] [ ]
    */
    int offsets[8][2] = {
        {1, 1}, {1, 0}, {1, -1},
        {0, 1},         {0, -1},
        {-1, 1},{-1, 0},{-1, -1}
    };

    // Check if moving to the target square causes the kings to touch
    for (int i = 0; i < BOARD_SIZE; ++i) {
        int check_y = target.rank - 1 + offsets[i][0];
        int check_x = target.file - 1 + offsets[i][1];

        // Ensure the coordinates are within bounds
        if (check_y >= 0 && check_y < BOARD_SIZE && check_x >= 0 && check_x < BOARD_SIZE) {
            if (board->grid[check_y][check_x] == other_king) {
                return 1;
            }
        }
    }
    return 0;
}

_Bool king_can_move(board_t *board, square_t piece, square_t target)
{
    char _piece = board->grid[COORDS(piece)];
    int color = piece_color(_piece);

    // Validate that the piece is a king
    if (tolower(_piece) != 'k') {
        DEBU("Piece is not a king");
        board->state.error = ERROR_INVALID_PIECE;
        return 0;
    }

    // Validate there is a piece at the source square
    if (color == PIECE_COLOR_NONE) {
        DEBU("No piece found at: %s", piece.name);
        board->state.error = ERROR_EMPTY_SQUARE;
        return 0;
    }

    // Calculate file (horizontal) and rank (vertical) differences
    int file_diff = abs((int)target.file - (int)piece.file);
    int rank_diff = abs((int)target.rank - (int)piece.rank);

    // Kings move one square in any direction
    if (!(file_diff == 2 && rank_diff == 0) && !(file_diff <= 1 && rank_diff <= 1 && (file_diff + rank_diff > 0))) {
        DEBU("Invalid king move");
        board->state.error = ERROR_INVALID_MOVE;
        return 0;
    }

    // Check if the target square contains a piece of the same color
    char target_piece = board->grid[COORDS(target)];
    if (target_piece != EMPTY_SQUARE && piece_color(target_piece) == color) {
        DEBU("Target square contains a piece of the same color");
        board->state.error = ERROR_FRIENDLY_PIECE;
        return 0;
    }

    if(square_is_attacked(board, target, !color)) {
        DEBU("King would be in check");
        board->state.error = ERROR_INVALID_MOVE;
        return 0;
    }

    // Check if moving to the target square leads to the kings touching
    if(kings_touching(board, target, color)){
        DEBU("Kings cannot touch");
        board->state.error = ERROR_KINGS_TOUCHING;
        return 0;
    }

    // Castling logic
    if (file_diff == 2 && rank_diff == 0) {

        if(IN_CHECK(board, color)) return 0;

        if (color == PIECE_COLOR_WHITE) {
            // White king castling
            if (target.file > piece.file) {
                // Kingside castling
                if (!(board->state.castling_rights & CASTLE_WHITE_KINGSIDE)) {
                    DEBU("White kingside castling not allowed");
                    board->state.error = ERROR_INVALID_CASTLE;
                    return 0;
                }
                // Ensure squares between king and rook are empty
                if (board->grid[piece.rank - 1][5] != EMPTY_SQUARE || board->grid[piece.rank - 1][6] != EMPTY_SQUARE) {
                    DEBU("Path to white kingside castling is not clear");
                    board->state.error = ERROR_OBSTRUCTED_PATH;
                    return 0;
                }
            } else {
                // Queenside castling
                if (!(board->state.castling_rights & CASTLE_WHITE_QUEENSIDE)) {
                    DEBU("White queenside castling not allowed");
                    board->state.error = ERROR_INVALID_CASTLE;
                    return 0;
                }
                // Ensure squares between king and rook are empty
                if (board->grid[piece.rank - 1][1] != EMPTY_SQUARE || board->grid[piece.rank - 1][2] != EMPTY_SQUARE || board->grid[piece.rank - 1][3] != EMPTY_SQUARE) {
                    DEBU("Path to white queenside castling is not clear");
                    board->state.error = ERROR_OBSTRUCTED_PATH;
                    return 0;
                }
            }
        } else {
            // Black king castling
            if (target.file > piece.file) {
                // Kingside castling
                if (!(board->state.castling_rights & CASTLE_BLACK_KINGSIDE)) {
                    DEBU("Black kingside castling not allowed");
                    board->state.error = ERROR_INVALID_CASTLE;
                    return 0;
                }
                // Ensure squares between king and rook are empty
                if (board->grid[piece.rank - 1][5] != EMPTY_SQUARE || board->grid[piece.rank - 1][6] != EMPTY_SQUARE) {
                    DEBU("Path to black kingside castling is not clear");
                    board->state.error = ERROR_OBSTRUCTED_PATH;
                    return 0;
                }
            } else {
                // Queenside castling
                if (!(board->state.castling_rights & CASTLE_BLACK_QUEENSIDE)) {
                    DEBU("Black queenside castling not allowed");
                    board->state.error = ERROR_INVALID_CASTLE;
                    return 0;
                }
                // Ensure squares between king and rook are empty
                if (board->grid[piece.rank - 1][1] != EMPTY_SQUARE || board->grid[piece.rank - 1][2] != EMPTY_SQUARE || board->grid[piece.rank - 1][3] != EMPTY_SQUARE) {
                    DEBU("Path to black queenside castling is not clear");
                    board->state.error = ERROR_OBSTRUCTED_PATH;
                    return 0;
                }
            }
        }


        // Ensure the king and rook do not pass through or land on attacked squares
        // Determine castling direction (kingside or queenside)
        int castle_direction = (target.x > piece.x) ? 1 : -1;

        if (square_is_attacked_coords(board, piece.y, piece.x + castle_direction, !color) ||
                square_is_attacked_coords(board, target.y, target.x, !color)) {
            DEBU("Cannot castle through or into check");
            board->state.error = ERROR_INVALID_CASTLE;
            return 0;
        }

        // Check if the target square (final destination) leads to a king touching scenario
        if (kings_touching(board, target, color)) {
            DEBU("Kings cannot touch");
            board->state.error = ERROR_KINGS_TOUCHING;
            return 0;
        }
    }

    board->state.error = 0;
    return 1; // Move is valid
}

_Bool king_is_castling(const board_t* board, square_t from, square_t to)
{
    char piece = piece_at((board_t*) board, from);

    if (tolower(piece) != 'k')  return 0;

    int color = piece_color(piece);

    return strcmp(from.name, (color == PIECE_COLOR_WHITE) ? "e1" : "e8") == 0 &&
        abs((int)from.file - (int)to.file) == 2;
}

_Bool king_can_castle(board_t* board, square_t from, square_t to)
{
    // King is not in a starting square
    if (strcmp(from.name, "e1") && strcmp(from.name, "e8")) {
        DEBU("Not on a starting square");
        return 0;
    }


    char piece = piece_at(board, from);
    int color = piece_color(piece);
    int file_diff = (int) from.file - (int) to.file;

    if(IN_CHECK(board, color)) {
        DEBU("In check");
        return 0;
    }

    if (color == PIECE_COLOR_WHITE) {
        return (file_diff == -2 && has_castling_rights(board, CASTLE_WHITE_KINGSIDE)) ||
               (file_diff == 2 && has_castling_rights(board, CASTLE_WHITE_QUEENSIDE));
    } else if (color == PIECE_COLOR_BLACK) {
        return (file_diff == -2 && has_castling_rights(board, CASTLE_BLACK_KINGSIDE)) ||
               (file_diff == 2 && has_castling_rights(board, CASTLE_BLACK_QUEENSIDE));
    }

    return 0;
}

void king_castle(board_t* board, square_t from, square_t to)
{
    char piece = piece_at(board, from);
    int color = piece_color(piece);
    int file_diff = (int) from.file - (int) to.file;

    square_t kingside_rook, queenside_rook, kingside_rook_target, queenside_rook_target;
    square_from_name(&kingside_rook, color == PIECE_COLOR_WHITE ? "h1" : "h8");
    square_from_name(&queenside_rook, color == PIECE_COLOR_WHITE ? "a1" : "a8");
    square_from_name(&kingside_rook_target, color == PIECE_COLOR_WHITE ? "f1" : "f8");
    square_from_name(&queenside_rook_target, color == PIECE_COLOR_WHITE ? "d1" : "d8");

    DEBU("filediff: %d", file_diff);
    move_freely(board, from, to);
    if(file_diff == -2){
        move_freely(board, kingside_rook, kingside_rook_target);
    } else if(file_diff == 2){
        move_freely(board, queenside_rook, queenside_rook_target);
    }
}
