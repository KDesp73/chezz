#include "board.h"
#include "piece.h"
#include "square.h"
#include "tests.h"
#include "extern/test.h"

int test_is_pinned(const char* fen, const char* square, _Bool expected)
{
    board_t board;
    board_init_fen(&board, fen);

    square_t _square;
    square_from_name(&_square, square);

    if(expected != piece_is_pinned(&board, _square)){
        FAIL("At fen %s square %s. Expected %d. Found %d", fen, square, expected , !expected);
        return 0;
    }

    SUCC("Passed for fen %s square %s", fen, square);
    return 1;
}
