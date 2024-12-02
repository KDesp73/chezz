#include "board.h"
#include "move.h"
#include "piece.h"
#include "square.h"
#include <ctype.h>

_Bool is_checkmate(board_t* board)
{ 

    size_t valid_count;
    square_t** moves = valid_moves(board, find_king(board, PIECE_COLOR_WHITE), &valid_count);
    if(IN_CHECK(board, PIECE_COLOR_WHITE) && valid_count == 0) {
        squares_free(&moves, valid_count);
        return 1;
    }

    squares_free(&moves, valid_count);
    moves = valid_moves(board, find_king(board, PIECE_COLOR_BLACK), &valid_count);
    if(IN_CHECK(board, PIECE_COLOR_BLACK) && valid_count == 0) {
        squares_free(&moves, valid_count);
        return 1;
    }

    squares_free(&moves, valid_count);
    return 0;
}

_Bool is_stalemate_color(board_t* board, int color)
{
    size_t valid_count;

    // Check if the king is in check
    if (IN_CHECK(board, color)) {
        return 0; // Not a stalemate if the king is in check
    }

    // Iterate through all squares to find pieces of the given color
    for (int rank = 0; rank < BOARD_SIZE; rank++) {
        for (int file = 0; file < BOARD_SIZE; file++) {
            square_t* square = square_from_coords(rank, file);
            char piece = piece_at(board, square);

            // Check if the piece belongs to the player
            if (piece != EMPTY_SQUARE && piece_color(piece) == color) {
                square_t** moves = valid_moves(board, square, &valid_count);

                // If any valid move exists, it's not a stalemate
                if (valid_count > 0) {
                    squares_free(&moves, valid_count);
                    return 0;
                }

                squares_free(&moves, valid_count);
            }
        }
    }

    // No valid moves and the king is not in check => Stalemate
    return 1;
}

_Bool is_stalemate(board_t* board)
{
    return is_stalemate_color(board, PIECE_COLOR_WHITE) ||
        is_stalemate_color(board, PIECE_COLOR_BLACK);
}

_Bool is_threefold_repetition(board_t* board);

_Bool is_insufficient_material(board_t* board)
{
    int white_bishops = 0, black_bishops = 0;
    int white_knights = 0, black_knights = 0;
    int white_pieces = 0, black_pieces = 0;

    for (int rank = 0; rank < BOARD_SIZE; rank++) {
        for (int file = 0; file < BOARD_SIZE; file++) {
            square_t* square = square_from_coords(rank, file);
            char piece = piece_at(board, square);

            if (piece == EMPTY_SQUARE) continue;

            int color = piece_color(piece);
            switch (tolower(piece)) {
                case 'p':
                case 'q':
                case 'r':
                    // Pawns, queens, and rooks can checkmate, so return early
                    return 0;

                case 'b':
                    if (color == PIECE_COLOR_WHITE) {
                        white_bishops++;
                    } else {
                        black_bishops++;
                    }
                    break;

                case 'n':
                    if (color == PIECE_COLOR_WHITE) {
                        white_knights++;
                    } else {
                        black_knights++;
                    }
                    break;

                default:
                    // Count kings or invalid pieces
                    if (color == PIECE_COLOR_WHITE) {
                        white_pieces++;
                    } else {
                        black_pieces++;
                    }
            }
        }
    }

    // Count total pieces for each player
    int total_white = white_bishops + white_knights + white_pieces;
    int total_black = black_bishops + black_knights + black_pieces;

    // Insufficient material cases:
    // 1. Two lone kings
    if (total_white == 1 && total_black == 1) {
        return 1;
    }

    // 2. A lone king vs king and bishop
    if ((total_white == 1 && total_black == 2 && black_bishops == 1) ||
        (total_black == 1 && total_white == 2 && white_bishops == 1)) {
        return 1;
    }

    // 3. A lone king vs king and knight
    if ((total_white == 1 && total_black == 2 && black_knights == 1) ||
        (total_black == 1 && total_white == 2 && white_knights == 1)) {
        return 1;
    }

    // 4. King and knight vs king and bishop
    if (total_white == 2 && total_black == 2 &&
        white_knights == 1 && black_bishops == 1) {
        return 1;
    }

    if (total_black == 2 && total_white == 2 &&
        black_knights == 1 && white_bishops == 1) {
        return 1;
    }

    // 5. King and knight vs king and knight
    if (total_white == 2 && total_black == 2 &&
        white_knights == 1 && black_knights == 1) {
        return 1;
    }

    // 6. King and bishop vs king and bishop
    if (total_white == 2 && total_black == 2 &&
        white_bishops == 1 && black_bishops == 1) {
        return 1;
    }

    // 7. A lone king vs king and 2 knights
    if ((total_white == 1 && total_black == 3 && black_knights == 2) ||
        (total_black == 1 && total_white == 3 && white_knights == 2)) {
        return 1;
    }

    // Not insufficient material
    return 0;
}
