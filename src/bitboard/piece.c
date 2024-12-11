#include "piece.h"
#include "board.h"
#include "zobrist.h"

Piece PieceAt(const Board* board, Square square)
{
    Piece pieceLookup[PIECE_TYPES] = {
        [INDEX_BLACK_KING]   = (Piece){.type = 'k', .color = PIECE_COLOR_BLACK},
        [INDEX_BLACK_QUEEN]  = (Piece){.type = 'q', .color = PIECE_COLOR_BLACK},
        [INDEX_BLACK_ROOK]   = (Piece){.type = 'r', .color = PIECE_COLOR_BLACK},
        [INDEX_BLACK_BISHOP] = (Piece){.type = 'b', .color = PIECE_COLOR_BLACK},
        [INDEX_BLACK_KNIGHT] = (Piece){.type = 'n', .color = PIECE_COLOR_BLACK},
        [INDEX_BLACK_PAWN]   = (Piece){.type = 'p', .color = PIECE_COLOR_BLACK},
        [INDEX_WHITE_KING]   = (Piece){.type = 'K', .color = PIECE_COLOR_WHITE},
        [INDEX_WHITE_QUEEN]  = (Piece){.type = 'Q', .color = PIECE_COLOR_WHITE},
        [INDEX_WHITE_ROOK]   = (Piece){.type = 'R', .color = PIECE_COLOR_WHITE},
        [INDEX_WHITE_BISHOP] = (Piece){.type = 'B', .color = PIECE_COLOR_WHITE},
        [INDEX_WHITE_KNIGHT] = (Piece){.type = 'N', .color = PIECE_COLOR_WHITE},
        [INDEX_WHITE_PAWN]   = (Piece){.type = 'P', .color = PIECE_COLOR_WHITE}
    };

    for (size_t i = 0; i < PIECE_TYPES; i++) {
        if (board->bitboards[i] & (1ULL << (square))) {
            return pieceLookup[i];
        }
    }

    return (Piece){.type = ' ', .color = PIECE_COLOR_NONE};
}

