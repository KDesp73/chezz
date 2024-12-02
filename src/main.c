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
            ERRO("Invalid square(s). From: %s, To: %s", from, to);
            PRINT_FULL(&board, NULL);
            continue;
        }

        char piece = piece_at(&board, from_square);
        if (piece == EMPTY_SQUARE) {
            clib_ansi_clear_screen();
            printf("No piece at %s.\n", from);
            PRINT_FULL(&board, NULL);
            printf("%s's turn\n", board.turn ? "White" : "Black");
            square_free(&from_square);
            square_free(&to_square);
            continue;
        }

        if (!move_is_valid(&board, from_square, to_square)) {
            clib_ansi_clear_screen();
            PRINT_FULL(&board, NULL);
            square_free(&from_square);
            square_free(&to_square);
            continue;
        }

        // Execute the move
        if(king_is_castling(&board, from_square, to_square) && can_castle(&board, from_square, to_square)){
            castle(&board, from_square, to_square);
        } else {
            move(&board, from_square, to_square);
        }

        board.turn = !board.turn;

        // Update check status
        if (IN_CHECK(&board, PIECE_COLOR_WHITE)) {
            board.checks |= CHECK_WHITE_KING;
        } else {
            board.checks &= ~CHECK_WHITE_KING;
        }
        if (IN_CHECK(&board, PIECE_COLOR_BLACK)) {
            board.checks |= CHECK_BLACK_KING;
        } else {
            board.checks &= ~CHECK_BLACK_KING;
        }

        clib_ansi_clear_screen();
        PRINT_FULL(&board, from_square, to_square, NULL);

        // Free resources
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
            TEST_VALID_MOVES,
            TEST_MOVE_IS_VALID,
            END
        );
    }

    run();

    return 0;
}
