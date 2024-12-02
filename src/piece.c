#include "piece.h"
#include "board.h"
#include "extern/clib.h"
#include "move.h"
#include "square.h"
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

char piece_at(const board_t* board, const square_t* square)
{
    return board->grid[PCOORDS(square)];
}

int piece_color(char piece)
{
    if(piece == ' ') return PIECE_COLOR_NONE;

    return (piece < 'a') == PIECE_COLOR_WHITE;
}

_Bool piece_can_attack(board_t* board, const square_t* attacker, const square_t* target)
{
    if (!board || !attacker || !target) {
        return false;
    }

    char piece = board->grid[PCOORDS(attacker)];

    // Check if the square is empty
    if (piece == ' ') {
        ERRO("No piece in square %s", attacker->name);
        board->error = ERROR_EMPTY_SQUARE;
        return false;
    }

    // Pawn-specific attack logic
    if (tolower(piece) == 'p') {
        return pawn_can_attack(board, attacker, target);
    }

    // Other pieces
    return piece_can_move(board, attacker, target);
}

int count_attacking_pieces(board_t* board, const square_t* king, int attacker_color)
{
    int count = 0;

    // Loop through all squares on the board
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            if (piece_color(board->grid[i][j]) == attacker_color &&
                piece_can_attack(board, square_from_coords(i, j), king)) {
                ++count;
            }
        }
    }
    return count;
}

_Bool piece_is_pinned(board_t* board, const square_t* piece)
{
    if (!piece || !board) {
        ERRO("Invalid arguments to piece_is_pinned");
        return 0;
    }

    int color = piece_color(board->grid[PCOORDS(piece)]);
    square_t* king = find_king(board, color);
    if (!king) {
        ERRO("Could not find the %s king", (color == PIECE_COLOR_WHITE) ? "white" : "black");
        return 0;
    }
    int original_attackers = count_attacking_pieces(board, king, !color);

    // Temporarily remove the piece from the board
    char original_piece = board->grid[PCOORDS(piece)];
    board->grid[PCOORDS(piece)] = ' ';

    // Check if the king becomes attacked and by how many pieces
    int attackers = count_attacking_pieces(board, king, !color);

    // Restore the piece on the board
    board->grid[PCOORDS(piece)] = original_piece;

    square_free(&king);

    // The piece is pinned if removing it results to more attackers than before
    return (attackers > original_attackers);
}
