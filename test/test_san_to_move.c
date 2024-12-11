#include "board.h"
#include "extern/clib.h"
#include "notation.h"
#include "square.h"
#include "grid-tests.h"
#include "extern/test.h"
#include <string.h>

int test_san_to_move(const char* fen, const char* san, const char* expected_move)
{
    board_t board;
    board_init_fen(&board, fen);

    san_move_t san_move;
    strcpy(san_move.move, san);

    char from[3], to[3], expected_promotion;
    strncpy(from, expected_move, 2);
    from[2] = '\0';
    strncpy(to, expected_move + 2, 2);
    to[2] = '\0';
    expected_promotion = expected_move[4];

    square_t expected_from_square, expected_to_square;
    square_from_name(&expected_from_square, from);
    square_from_name(&expected_to_square, to);
    
    square_t found_from_square, found_to_square;
    char found_promotion;
    san_to_move(&board, san_move, &found_from_square, &found_to_square, &found_promotion);
    board_free(&board);

    if(!square_cmp(expected_from_square, found_from_square)){
        FAIL("At fen %s, san %s. Invalid from square. Expected %s. Found %s%s%c", fen, san, expected_move, found_from_square.name, found_to_square.name, found_promotion);
        return 0;
    }
    if(!square_cmp(expected_to_square, found_to_square)){
        FAIL("At fen %s, san %s. Invalid to square. Expected %s. Found %s%s%c", fen, san, expected_move, found_from_square.name, found_to_square.name, found_promotion);
        return 0;
    }
    if(expected_promotion != found_promotion){
        FAIL("At fen %s, san %s. Invalid promotion. Expected %s. Found %s%s%c", fen, san, expected_move, found_from_square.name, found_to_square.name, found_promotion);
        return 0;
    }

    SUCC("Passed for fen %s, san %s", fen, san);
    return 1;
}
