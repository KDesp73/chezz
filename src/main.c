#include "board.h"
#include "move.h"
#include "piece.h"
#define CLIB_IMPLEMENTATION
#include "extern/clib.h"
#define TEST_IMPLEMENTATION
#include "extern/test.h"
#include "square.h"
#include <stdio.h>
#include <stdlib.h>
#include "tests.h"

void move_and_print(board_t* board, const char* from, const char* to)
{
    move(board, from, to);
    size_t count;
    square_t** s = squares(&count,
                    square_from_name(from),
                    square_from_name(to),
                    NULL
                );

    board_print_highlight(board, s, count);
}

int main(int argc, char** argv){
    if(argc == 2 && STREQ(argv[1], "test")){
        return !test(
            // TEST_SQUARE_FROM_NAME,
            // TEST_PAWN_MOVE,
            // TEST_ROOK_MOVE,
            // TEST_BISHOP_MOVE,
            // TEST_QUEEN_MOVE,
            // TEST_KNIGHT_MOVE,
            TEST_KING_MOVE,
            END
        );
    }

    board_t board;
    board_init_fen(&board, "2r1k1r1/8/8/8/8/8/8/R3K2R w KQkq - 0 1");
    board_print(&board);

    return 0;
}
