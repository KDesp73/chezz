#include "bitboard-tests.h"
#include "board.h"
#include "ui.h"
#include "grid-tests.h"
#include "extern/test.h"

int test_checkmate(const char* fen, _Bool expected)
{
    board_t board;
    board_init_fen(&board, fen);

    if(expected != is_checkmate(&board)){
        PRINT_PLAIN(&board);
        FAIL("For fen %s. Expected %d. Found %d", fen, expected, !expected);
        return 0;
    }

    SUCC("Passed for fen %s", fen);
    return 1;
}

int TestCheckmate(const char* fen, _Bool expected)
{
    Board board;
    BoardInitFen(&board, fen);

    if(expected != IsCheckmate(&board)){
        TuiBoardPrint(&board, MINIMAL_CONFIG, 64);
        FAIL("For fen %s. Expected %d. Found %d", fen, expected, !expected);
        return 0;
    }

    SUCC("Passed for fen %s", fen);
    return 1;
}
