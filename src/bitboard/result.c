#include "board.h"
#include "piece.h"

_Bool IsCheckmateColor(Board* board, uint8_t color)
{
    // uint64_t kingBitboard = board->bitboards[color == PIECE_COLOR_BLACK ? INDEX_BLACK_KING : INDEX_WHITE_KING];
    // 
    // if (kingBitboard == 0) {
    //     return 0;
    // }
    //
    // int kingSquare = __builtin_ctzll(kingBitboard);
    //
    // // 1. Check if the King is in check (Check if any opposing pieces can attack the king)
    // if (IsKingInCheck(*board, color)) {
    //     // 2. Check if the king has any legal moves to escape the check
    //     if (HasEscapeMoves(board, color, kingSquare)) {
    //         return 0; // Not checkmate, the king has escape moves
    //     }
    //
    //     // 3. Check if any piece can block or capture the attacking piece
    //     if (CanBlockOrCaptureAttacker(board, color, kingSquare)) {
    //         return 0;
    //     }
    //
    //     return 1;
    // }

    return 0;
}

_Bool IsCheckmate(Board* board)
{
    return IsCheckmateColor(board, PIECE_COLOR_WHITE)
        || IsCheckmateColor(board, PIECE_COLOR_BLACK);
}

_Bool IsStalemate(Board* board)
{

}

_Bool IsInsufficientMaterial(Board* board)
{

}

_Bool IsThreefoldRepetition(Board* board)
{

}
