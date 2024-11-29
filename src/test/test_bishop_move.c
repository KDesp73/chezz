#include "board.h"
#include "extern/test.h"
#include "move.h"
#include "square.h"
#include "tests.h"
#include <string.h>

int test_bishop_can_move(const char* fen, const char* from, const char* to, _Bool expected)
{
    board_t board;
    board_init_fen(&board, fen);
    // size_t count;
    // square_t** s = squares(&count, 
    //         square_from_name(from),
    //         square_from_name(to),
    //         NULL);
    // board_print_highlight(&board, s, count);

    _Bool found = bishop_can_move(&board, square_from_name(from), square_from_name(to));
    if(expected != found){
        FAIL("At: [fen %s] [from %s] [to %s]. Expected %d. Found %d", fen, from, to, expected, found);
        return 0;
    }

    SUCC("At: [fen %s] [from %s] [to %s]. Expected %d. Found %d", fen, from, to, expected, found);
    return 1;
}
