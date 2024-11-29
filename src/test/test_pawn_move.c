#include "board.h"
#include "extern/test.h"
#include "move.h"
#include "square.h"
#include "tests.h"

int test_pawn_can_move(const char* fen, const char* from, const char* to, _Bool expected)
{
    board_t board;
    board_init_fen(&board, fen);

    _Bool found = pawn_can_move(&board, square_from_name(from), square_from_name(to));
    if(expected != found){
        FAIL("At: [fen %s] [from %s] [to %s]. Expected %d. Found %d", fen, from, to, expected, found);
        return 0;
    }

    SUCC("At: [fen %s] [from %s] [to %s]. Expected %d. Found %d", fen, from, to, expected, found);
    return 1;
}
