#include "board.h"
#include "extern/clib.h"
#include "hashing.h"
#include "move.h"
#include "piece.h"
#include "square.h"
#include "zobrist.h"
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#undef DEBU
#define DEBU(fmt, ...)

_Bool is_checkmate_color(board_t* board, int color)
{
    assert(board != NULL);

    // Find the king of the given color
    square_t king;
    find_king(&king, board, color);

    // Check if the king has any valid moves
    size_t valid_count = 0;
    square_t** king_moves = valid_moves(board, king, &valid_count);
    squares_free(&king_moves, valid_count);

    if (!IN_CHECK(board, color) || valid_count > 0) {
        return 0; // Not checkmate if the king is not in check or has valid moves
    }

    // Determine the attackers on the king
    size_t attackers_count = 0;
    square_t** attackers = square_is_attacked_by(board, king, !color, &attackers_count);

    if (attackers_count > 1) {
        squares_free(&attackers, attackers_count);
        DEBU("Multiple attackers, checkmate");
        return 1; // Checkmate if multiple attackers
    }

    if (attackers_count == 0 || attackers[0] == NULL) {
        squares_free(&attackers, attackers_count);
        DEBU("No attackers, not checkmate");
        return 0; // No attackers, not checkmate
    }

    square_t* attacker = attackers[0]; // Single attacker
    size_t my_attackers_count = 0;
    square_t** my_attackers = square_is_attacked_by(board, *attacker, color, &my_attackers_count);

    if (my_attackers_count > 0) {
        squares_free(&attackers, attackers_count);
        squares_free(&my_attackers, my_attackers_count);
        DEBU("We can capture the attacker, not checkmate");
        return 0; // Not checkmate if we can capture the attacker
    }

    // Check if the attack can be blocked
    size_t block_count = 0;
    square_t** attack_path = attack_path_to_king(board, king, *attacker, &block_count);

    if (block_count > 0) {
        for (size_t i = 0; i < block_count; i++) {
            size_t blockers_count = 0;
            square_t** blockers = square_is_accessible_by(board, *attack_path[i], color, &blockers_count);

            if (blockers_count > 0) {
                // We can block the attack
                squares_free(&blockers, blockers_count);
                squares_free(&attack_path, block_count);
                squares_free(&attackers, attackers_count);
                squares_free(&my_attackers, my_attackers_count);
                DEBU("Check can be blocked, not checkmate");
                return 0;
            }

            squares_free(&blockers, blockers_count);
        }
    }

    squares_free(&attack_path, block_count);
    squares_free(&attackers, attackers_count);
    squares_free(&my_attackers, my_attackers_count);

    // // Special case: attacker is a king
    // if (tolower(piece_at(board, *attacker)) == 'k') {
    //     DEBU("Attacker is a king, checkmate");
    //     return 1;
    // }

    DEBU("No way to capture or block, checkmate");
    return 1; // Checkmate (no way to capture or block)
}

_Bool is_checkmate(board_t *board)
{
    return is_checkmate_color(board, PIECE_COLOR_WHITE) ||
        is_checkmate_color(board, PIECE_COLOR_BLACK);
}

_Bool is_stalemate_color(board_t* board, int color)
{
    size_t valid_count;

    if (IN_CHECK(board, color)) {
        return 0; // Not a stalemate if the king is in check
    }

    // Iterate through all squares to find pieces of the given color
    for (int rank = 0; rank < BOARD_SIZE; rank++) {
        for (int file = 0; file < BOARD_SIZE; file++) {
            square_t square;
            square_from_coords(&square, rank, file);
            char piece = piece_at(board, square);

            // Check if the piece belongs to the player
            if (piece != EMPTY_SQUARE && piece_color(piece) == color) {
                square_t** moves = valid_moves(board, square, &valid_count);
                squares_free(&moves, valid_count);

                // If any valid move exists, it's not a stalemate
                if (valid_count > 0) {
                    return 0;
                }
            }
        }
    }

    // No valid moves and the king is not in check => Stalemate
    return 1;
}

_Bool is_stalemate(board_t* board)
{
    return (board->state.turn == PIECE_COLOR_WHITE && is_stalemate_color(board, PIECE_COLOR_WHITE)) ||
        (board->state.turn == PIECE_COLOR_BLACK && is_stalemate_color(board, PIECE_COLOR_BLACK));
}

_Bool is_threefold_repetition(board_t* board)
{
    uint64_t hash = calculate_zobrist_hash(board);
    return update_hash_table(&board->state.history, hash);
}

_Bool is_insufficient_material(board_t* board)
{
    int white_bishops = 0, black_bishops = 0;
    int white_knights = 0, black_knights = 0;
    int white_pieces = 0, black_pieces = 0;

    for (int rank = 0; rank < BOARD_SIZE; rank++) {
        for (int file = 0; file < BOARD_SIZE; file++) {
            square_t square;
            square_from_coords(&square, rank, file);
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
