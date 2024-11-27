#include "board.h"
#define CLIB_IMPLEMENTATION
#include "extern/clib.h"
#define TEST_IMPLEMENTATION
#include "extern/test.h"
#include "square.h"
#include <stdio.h>
#include <stdlib.h>
#include "tests.h"

int main(int argc, char** argv){
    if(argc == 2 && STREQ(argv[1], "test")){
        return !test(
            TEST_SQUARE_FROM_NAME,
            END
        );
    }

    board_t board;
    board_init(&board);

    size_t count;
    square_t** s = squares(&count,
                    square_from_name("e1"),
                    square_from_name("h8"),
                    NULL
                );

    board_print_highlight(&board, s, count);

    return 0;
}
