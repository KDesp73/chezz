#include "zobrist.h"
#define TEST_IMPLEMENTATION
#include "extern/test.h"
#include "tests.h"

int main(int argc, char** argv){
    init_zobrist();

    return !test(
        TEST_SQUARE_FROM_NAME,
        TEST_PAWN_MOVE,
        TEST_ROOK_MOVE,
        TEST_BISHOP_MOVE,
        TEST_QUEEN_MOVE,
        TEST_KNIGHT_MOVE,
        TEST_KING_MOVE,
        TEST_IS_PINNED,
        TEST_PAWN_IS_ENPASSANTING,
        TEST_PAWN_CAN_ENPASSANT,
        TEST_MOVE,
        TEST_KING_IN_CHECK,
        TEST_BOARD_INIT_FEN,
        TEST_INSUFFICIENT_MATERIAL,
        TEST_MOVE_IS_VALID,
        TEST_VALID_MOVES,
        TEST_STALEMATE,
        TEST_CHECKMATE,
        TEST_IS_ATTACKED_BY,
        TEST_MOVE_TO_SAN,
        TEST_SAN_TO_MOVE,
        END
    );
}
