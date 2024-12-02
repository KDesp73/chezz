
#include "board.h"
#include "tests.h"
#include "extern/test.h"

int test_insufficient_material(const char* fen, _Bool expected)
{
    board_t board;
    board_init_fen(&board, fen);

    if(expected != is_insufficient_material(&board)){
        FAIL("For fen %s. Expected %d. Found %d", fen, expected, !expected);
        return 0;
    }

    SUCC("Passed for fen %s", fen);
    return 1;
}
