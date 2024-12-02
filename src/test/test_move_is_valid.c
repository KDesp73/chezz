#include "board.h"
#include "move.h"
#include "square.h"
#include "tests.h"
#include "extern/test.h"

int test_move_is_valid(const char* fen, const char* from, const char* to, _Bool expected)
{
    board_t board;
    board_init_fen(&board, fen);

    if(expected != move_is_valid(&board, square_from_name(from), square_from_name(to))){
        FAIL("For fen %s, move %s%s. Expected %d. Found %d", fen, from, to, expected, !expected);
        return 0;
    }

    SUCC("For fen %s, move %s%s", fen, from, to);
    return 1;
}
