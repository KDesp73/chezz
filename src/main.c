#include "board.h"
#include "move.h"
#include "piece.h"
#include <string.h>
#define CLIB_IMPLEMENTATION
#include "extern/clib.h"
#define TEST_IMPLEMENTATION
#include "extern/test.h"
#include "square.h"
#include <stdio.h>
#include <stdlib.h>
#include "tests.h"

void run()
{
    board_t board;
    board_init(&board);
    board_print(&board);
    printf("%s's turn\n", board.turn ? "White" : "Black");

    while(1) {

        char move_input[5]; // +1 for \0
        printf("Enter move: ");
        scanf("%4s", move_input);

        char from[3], to[3];
        strncpy(from, move_input, 2);
        from[2] = '\0';
        strncpy(to, move_input+2, 2);
        to[2] = '\0';

        square_t* from_square = square_from_name(from);
        square_t* to_square = square_from_name(to);
        if(!from_square || !to_square) {
            clib_ansi_clear_screen();
            ERRO("Invalid move. From: %s, To: %s", from, to);
            board_print(&board);
            printf("%s's turn\n", board.turn ? "White" : "Black");
            continue;
        }

        char piece = piece_at(&board, from_square);
        if(piece == EMPTY_SQUARE){
            clib_ansi_clear_screen();
            ERRO("Square %s is empty", from);
            board_print(&board);
            printf("%s's turn\n", board.turn ? "White" : "Black");
            continue;
        }

        if(board.turn != piece_color(piece)) {
            clib_ansi_clear_screen();
            ERRO("Cannot move opponent's pieces");
            board_print(&board);
            printf("%s's turn\n", board.turn ? "White" : "Black");
            continue;
        }

        if(!piece_can_move(&board, from_square, to_square)){
            clib_ansi_clear_screen();
            ERRO("Invalid move");
            board_print(&board);
            printf("%s's turn\n", board.turn ? "White" : "Black");
            continue;
        }
        
        move(&board, from_square, to_square);

        size_t highlight_count;
        square_t** sqrs = squares(&highlight_count,
                from_square,
                to_square,
                NULL);

        clib_ansi_clear_screen();
        board_print_highlight(&board, sqrs, highlight_count);

        board.turn = !board.turn;
        printf("%s's turn\n", board.turn ? "White" : "Black");

        square_free(&from_square);
        square_free(&to_square);
    }

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

    run();

    return 0;
}
