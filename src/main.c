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

void run(const char* fen)
{
    board_t board;
    board_init_fen(&board, fen);
    clib_ansi_clear_screen();
    PRINT_FULL(&board, NULL);

    while (1) {
        char move_input[6]; // +1 for \0, +1 for safety
        printf("Enter move (e.g., e2e4): ");
        if (scanf("%5s", move_input) != 1) {
            printf("Invalid input. Try again.\n");
            continue;
        }

        if (strlen(move_input) != 4) {
            printf("Invalid move format. Use 4 characters (e.g., e2e4).\n");
            continue;
        }

        char from[3], to[3];
        strncpy(from, move_input, 2);
        from[2] = '\0';
        strncpy(to, move_input + 2, 2);
        to[2] = '\0';

        square_t* from_square = square_from_name(from);
        square_t* to_square = square_from_name(to);

        if (!from_square || !to_square) {
            clib_ansi_clear_screen();
            PRINT_FULL(&board, NULL);
            continue;
        }

        char piece = piece_at(&board, from_square);
        if (piece == EMPTY_SQUARE) {
            clib_ansi_clear_screen();
            PRINT_FULL(&board, NULL);
            square_free(&from_square);
            square_free(&to_square);
            continue;
        }

        if(!move(&board, from_square, to_square)){
            clib_ansi_clear_screen();
            PRINT_FULL(&board, NULL);
            square_free(&from_square);
            square_free(&to_square);
            continue;
        }

        if(board.result > 0){
            printf("%s\n", result_message[board.result]);
            square_free(&from_square);
            square_free(&to_square);
            return;
        }

        clib_ansi_clear_screen();
        PRINT_FULL(&board, from_square, to_square, NULL);

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
            // TEST_KING_MOVE,
            // TEST_IS_PINNED,
            // TEST_VALID_MOVES,
            // TEST_MOVE_IS_VALID,
            // TEST_PAWN_IS_ENPASSANTING,
            // TEST_PAWN_CAN_ENPASSANT,
            // TEST_MOVE,
            // TEST_KING_IN_CHECK,
            TEST_BOARD_INIT_FEN,
            END
        );
    }

    const char* enpassant_fen = "rnbqkbnr/1pp1pppp/p7/3pP3/8/8/PPPP1PPP/RNBQKBNR w KQkq d6 0 1";
    run("8/1k5P/8/8/8/8/8/3K4 w - - 49 50");

    return 0;
}
