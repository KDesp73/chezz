#include "bitboard-tests.h"
#include "board.h"
#include "grid-tests.h"
#include "extern/test.h"

int test_stalemate(const char* fen, _Bool expected)
{
    board_t board;
    board_init_fen(&board, fen);

    if(expected != is_stalemate(&board)){
        FAIL("For fen %s. Expected %d. Found %d", fen, expected, !expected);
        return 0;
    }

    SUCC("Passed for fen %s", fen);
    return 1;
}

int TestStalemate(const char* fen, _Bool expected)
{
    Board board;
    BoardInitFen(&board, fen);

    if(expected != IsStalemate(&board)){
        FAIL("For fen %s. Expected %d. Found %d", fen, expected, !expected);
        return 0;
    }

    SUCC("Passed for fen %s", fen);
    return 1;
}
