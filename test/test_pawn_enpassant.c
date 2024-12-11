#include "bitboard-tests.h"
#include "board.h"
#include "move.h"
#include "square.h"
#include "grid-tests.h"
#include "extern/test.h"

int test_pawn_is_enpassanting(const char* fen, const char* from, const char* to, _Bool expected)
{
    board_t board;
    board_init_fen(&board, fen);

    square_t from_square, to_square;
    square_from_name(&from_square, from);
    square_from_name(&to_square, to);

    if(expected != pawn_is_enpassanting(&board, from_square, to_square)){
        FAIL("For fen %s, move %s%s. Expected %d. Found %d", fen, from, to, expected, !expected);
        return 0;
    }

    SUCC("Passed for fen %s, move %s%s", fen, from, to);
    return 1;
}

int test_pawn_can_enpassant(const char* fen, const char* from, const char* to, _Bool expected)
{
    board_t board;
    board_init_fen(&board, fen);

    square_t from_square, to_square;
    square_from_name(&from_square, from);
    square_from_name(&to_square, to);

    if(expected != pawn_can_enpassant(&board, from_square, to_square)){
        FAIL("For fen %s, move %s%s. Expected %d. Found %d", fen, from, to, expected, !expected);
        return 0;
    }

    SUCC("Passed for fen %s, move %s%s", fen, from, to);
    return 1;
}

int TestPawnIsEnpassanting(const char* fen, const char* from, const char* to, _Bool expected)
{
    return 1;
    Board board;
    BoardInitFen(&board, fen);

    Square from_square, to_square;
    from_square = SquareFromName(from);
    to_square = SquareFromName(to);

    if(expected != (&board, from_square, to_square)){
        FAIL("For fen %s, move %s%s. Expected %d. Found %d", fen, from, to, expected, !expected);
        return 0;
    }

    SUCC("Passed for fen %s, move %s%s", fen, from, to);
    return 1;
}

int TestPawnCanEnpassant(const char* fen, const char* from, const char* to, _Bool expected)
{
    return 1;
    board_t board;
    board_init_fen(&board, fen);

    square_t from_square, to_square;
    square_from_name(&from_square, from);
    square_from_name(&to_square, to);

    if(expected != pawn_can_enpassant(&board, from_square, to_square)){
        FAIL("For fen %s, move %s%s. Expected %d. Found %d", fen, from, to, expected, !expected);
        return 0;
    }

    SUCC("Passed for fen %s, move %s%s", fen, from, to);
    return 1;
}
