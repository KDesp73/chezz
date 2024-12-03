#include "piece.h"
#include "board.h"
#include "extern/clib.h"
#include "move.h"
#include "square.h"
#include <ctype.h>

char piece_at(const board_t* board, square_t square)
{
    return board->grid[COORDS(square)];
}

int piece_color(char piece)
{
    if(piece == ' ') return PIECE_COLOR_NONE;

    return (piece < 'a') == PIECE_COLOR_WHITE;
}

_Bool piece_can_attack(board_t* board, square_t attacker, square_t target, _Bool strict)
{
    if (!board) {
        return 0;
    }

    char piece = board->grid[COORDS(attacker)];

    // Check if the square is empty
    if (strict && piece == ' ') {
        ERRO("No piece in square %s", attacker.name);
        return 0;
    }

    // Pawn-specific attack logic
    if (tolower(piece) == 'p') {
        return pawn_can_attack(board, attacker, target, strict);
    }

    // Other pieces
    return piece_can_move(board, attacker, target);
}

int count_attacking_pieces(board_t* board, square_t king, int attacker_color)
{
    int count = 0;

    // Loop through all squares on the board
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            square_t square;
            square_from_coords(&square, i, j);
            if (piece_color(board->grid[i][j]) == attacker_color &&
                piece_can_attack(board, square, king, 1)) {
                ++count;
            }
        }
    }
    return count;
}

_Bool piece_is_pinned(board_t* board, square_t piece)
{
    if (!board) {
        ERRO("Invalid arguments to piece_is_pinned");
        return 0;
    }

    int color = piece_color(board->grid[COORDS(piece)]);
    square_t king;
    find_king(&king, board, color);

    int original_attackers = count_attacking_pieces(board, king, !color);

    // Temporarily remove the piece from the board
    char original_piece = board->grid[COORDS(piece)];
    board->grid[COORDS(piece)] = ' ';

    // Check if the king becomes attacked and by how many pieces
    int attackers = count_attacking_pieces(board, king, !color);

    // Restore the piece on the board
    board->grid[COORDS(piece)] = original_piece;

    // The piece is pinned if removing it results to more attackers than before
    return (attackers > original_attackers);
}
