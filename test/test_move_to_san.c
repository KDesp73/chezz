#include "board.h"
#include "extern/clib.h"
#include "move.h"
#include "notation.h"
#include "square.h"
#include "tests.h"
#include "extern/test.h"
#include "ui.h"
#include <string.h>

int test_move_to_san(const char* fen, const char* move, const char* expected)
{
    board_t board;
    board_init_fen(&board, fen);

    char from[3], to[3], promotion;
    strncpy(from, move, 2);
    from[2] = '\0';
    strncpy(to, move + 2, 2);
    to[2] = '\0';
    promotion = move[4];

    square_t from_square, to_square;
    square_from_name(&from_square, from);
    square_from_name(&to_square, to);

    if(!move_is_valid(&board, from_square, to_square)){
        FAIL("For fen %s, move %s. Invalid move", fen, move);
        return 0;
    }

    san_move_t san;
    move_to_san(&board, from_square, to_square, promotion, &san);

    if(!STREQ(san.move, expected)){
        FAIL("For fen %s, move %s. Expected %s. Found %s", fen, move, expected, san.move);
        return 0;
    }

    SUCC("Passed %s, move %s", fen, move);
    return 1;
}
