#include "bitboard-tests.h"
#include "board.h"
#include "move.h"
#include "square.h"
#include "grid-tests.h"
#include "extern/test.h"
#include "ui.h"

int test_move_is_valid(const char* fen, const char* from, const char* to, _Bool expected)
{
    board_t board;
    board_init_fen(&board, fen);

    square_t from_square, to_square;
    square_from_name(&from_square, from);
    square_from_name(&to_square, to);

    if(expected != move_is_valid(&board, from_square, to_square)){
        PRINT_PLAIN(&board);
        FAIL("For fen %s, move %s%s. Expected %d. Found %d", fen, from, to, expected, !expected);
        return 0;
    }

    SUCC("For fen %s, move %s%s", fen, from, to);
    return 1;
}

int TestMoveIsValid(const char* fen, const char* from, const char* to, _Bool expected)
{
    Board board;
    BoardInitFen(&board, fen);

    Square from_square, to_square;
    from_square = SquareFromName(from);
    to_square = SquareFromName(to);

    if(expected != MoveIsValid(&board, from_square, to_square)){
        TuiBoardPrint(&board, MINIMAL_CONFIG, 64);
        FAIL("For fen %s, move %s%s. Expected %d. Found %d", fen, from, to, expected, !expected);
        return 0;
    }

    SUCC("For fen %s, move %s%s", fen, from, to);
    return 1;
}
