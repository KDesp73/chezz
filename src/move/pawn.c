#include "board.h"
#include "extern/clib.h"
#include "move.h"
#include "piece.h"
#include "square.h"
#include <ctype.h>
#include <stdlib.h>

#undef DEBU
#define DEBU(format, ...)

_Bool pawn_can_move(board_t* board, square_t piece, square_t target)
{
    char _piece = board->grid[COORDS(piece)];
    int color = piece_color(_piece);

    // Validate that the piece is a pawn
    if (tolower(_piece) != 'p') {
        DEBU("Piece is not a pawn");
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
    int file_diff = abs((int)piece.file - (int)target.file);
    int rank_diff = (int)target.rank - (int)piece.rank;

    // Check movement direction based on color
    if ((color == PIECE_COLOR_WHITE && rank_diff <= 0) ||
        (color == PIECE_COLOR_BLACK && rank_diff >= 0)) {
        DEBU("Pawn moves backwards");
        board->error = ERROR_INVALID_MOVE;
        return 0;
    }

    // Pawns can only move forward 1 square (or 2 squares from starting position)
    if (abs(rank_diff) > 2 || abs(rank_diff) < 1) {
        DEBU("Pawn moves invalid distance");
        board->error = ERROR_INVALID_MOVE;
        return 0;
    }

    // Pawns can move 2 squares only from their starting rank
    if (abs(rank_diff) == 2) {
        if ((color == PIECE_COLOR_WHITE && piece.rank != 2) ||
            (color == PIECE_COLOR_BLACK && piece.rank != 7)) {
            DEBU("Pawn tries to move 2 squares from non-starting rank");
            board->error = ERROR_INVALID_MOVE;
            return 0;
        }

        square_t intermediate;
        square_from_square(&intermediate, target);
        square_set_rank(&intermediate, intermediate.rank + ((color == PIECE_COLOR_WHITE) ? -1 : 1));
        if (board->grid[COORDS(intermediate)] != ' ') {
            DEBU("Path is blocked for 2-square move");
            board->error = ERROR_OBSTRUCTED_PATH;
            return 0;
        }
    }

    // Check for diagonal capture
    if (abs(file_diff) == 1 && abs(rank_diff) == 1) {
        char target_piece = board->grid[COORDS(target)];

        square_t enpassant_square;
        square_from_name(&enpassant_square, board->enpassant_square);
        if(
            target_piece == ' ' && 
            board->enpassant_square[0] != '-' && 
            square_cmp(target, enpassant_square)
        ){
            board->error = 0;
            return 1;
        }

        if(target_piece == ' ') {
            board->error = ERROR_EMPTY_SQUARE;
            return 0;
        }

        if (piece_color(target_piece) == color) {
            DEBU("Invalid diagonal move");
            board->error = ERROR_FRIENDLY_PIECE;
            return 0;
        }
        // Valid capture
        board->error = 0;
        return 1;
    }

    // Ensure straight move has no horizontal displacement
    if (file_diff != 0) {
        DEBU("Invalid horizontal move");
        board->error = ERROR_INVALID_MOVE;
        return 0;
    }

    // Ensure target square is empty for straight moves
    if (board->grid[COORDS(target)] != ' ') {
        DEBU("Target square is occupied");
        board->error = ERROR_OBSTRUCTED_PATH;
        return 0;
    }

    // Move is valid
    board->error = 0;
    return 1;
}

_Bool pawn_can_attack(board_t* board, square_t piece, square_t target, _Bool strict)
{
    if (!board) {
        return 0;
    }

    char _piece = board->grid[COORDS(piece)];
    int color = piece_color(_piece);

    // Validate that the piece is a pawn
    if (tolower(_piece) != 'p') {
        board->error = ERROR_INVALID_PIECE;
        return 0;
    }

    // Validate there is a piece at the source square
    if (strict && _piece == ' ') {
        board->error = ERROR_EMPTY_SQUARE;
        return 0;
    }

    // Calculate file (horizontal) and rank (vertical) differences
    int file_diff = abs((int)piece.file - (int)target.file);
    int rank_diff = (int)target.rank - (int)piece.rank;

    // Check movement direction based on color
    if ((color == PIECE_COLOR_WHITE && rank_diff != 1) || 
        (color == PIECE_COLOR_BLACK && rank_diff != -1)) {
        board->error = ERROR_INVALID_MOVE;
        return 0;
    }

    // Check for diagonal capture
    if (file_diff == 1) {
        char target_piece = board->grid[COORDS(target)];

        // Handle en passant capture
        square_t enpassant_square;
        square_from_name(&enpassant_square, board->enpassant_square);
        if (target_piece == ' ' && 
            board->enpassant_square[0] != '-' && 
            square_cmp(target, enpassant_square)){
            board->error = 0;
            return 1;
        }

        // Standard capture: target square must contain an opponent's piece
        if (target_piece != ' ' && piece_color(target_piece) != color) {
            board->error = 0;
            return 1;
        }

        board->error = ERROR_EMPTY_SQUARE; // No valid piece to attack
        return 0;
    }

    // Any other move is invalid for attacking
    board->error = ERROR_INVALID_MOVE;
    return 0;
}

_Bool pawn_is_enpassanting(const board_t* board, square_t from, square_t to)
{
    if (tolower(piece_at(board, from)) != 'p')  return 0;

    int file_diff = abs((int)from.file - (int)to.file);
    char to_piece = board->grid[COORDS(to)];


    return (
        file_diff == 1 &&
        to_piece == ' ');
}

_Bool pawn_can_enpassant(const board_t* board, square_t from, square_t to)
{
    if (tolower(piece_at(board, from)) != 'p')  return 0;
    if(board->enpassant_square[0] == '-') return 0;

    square_t enpassant_square;
    square_from_name(&enpassant_square, board->enpassant_square);

    return square_cmp(to, enpassant_square);
}

void pawn_enpassant(board_t* board, square_t from, square_t to)
{
    if(board->enpassant_square[0] == '-') return;

    char _piece = board->grid[COORDS(from)];
    int color = piece_color(_piece);

    square_t enpassant_square, opponent_pawn;
    square_from_name(&enpassant_square, board->enpassant_square);
    square_from_coords(&opponent_pawn, (color == PIECE_COLOR_WHITE) ? 4 : 3, enpassant_square.x);

    board->grid[COORDS(opponent_pawn)] = ' ';
    move_freely(board, from, to);
}

