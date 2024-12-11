#include "board.h"
#include "grid-tests.h"
#include "extern/test.h"

int test_king_in_check(const char* fen, int color, _Bool expected)
{
    board_t board;
    board_init_fen(&board, fen);

    if(expected != IN_CHECK(&board, color)){
        FAIL("For fen %s, color %s", fen, color ? "white" : "black");
        return 0;
    }

    SUCC("Passed for fen %s, color %s", fen, color ? "white" : "black");
    return 1;
}
