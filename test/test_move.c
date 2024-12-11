#include "board.h"
#include "move.h"
#include "square.h"
#include "grid-tests.h"
#include "extern/test.h"

int test_move(const char* fen, const char* from, const char* to, _Bool expected)
{
    board_t board;
    board_init_fen(&board, fen);

    square_t from_square, to_square;
    square_from_name(&from_square, from);
    square_from_name(&to_square, to);

    if(expected != move(&board, from_square, to_square, 0)){
        FAIL("For fen %s, move %s%s. Expected %d. Found %d", fen, from, to, expected, !expected);
        return 0;
    }

    SUCC("Passed for fen %s, move %s%s", fen, from, to);
    return 1;
}
