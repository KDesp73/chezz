#include "board.h"
#include "extern/clib.h"
#include "extern/test.h"
#include "move.h"
#include "piece.h"
#include "square.h"
#include "tests.h"
#include "ui.h"

int test_pawn_can_move(const char* fen, const char* from, const char* to, _Bool expected)
{
    board_t board;
    board_init_fen(&board, fen);

    square_t from_square, to_square;
    square_from_name(&from_square, from);
    square_from_name(&to_square, to);

    _Bool found = pawn_can_move(&board, from_square, to_square);
    if(expected != found){
        FAIL("At: [fen %s] [from %s] [to %s]. Expected %d. Found %d", fen, from, to, expected, found);
        return 0;
    }

    SUCC("At: [fen %s] [from %s] [to %s]. Expected %d. Found %d", fen, from, to, expected, found);
    return 1;
}
