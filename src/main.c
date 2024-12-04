#include "board.h"
#include "move.h"
#include "notation.h"
#include "piece.h"
#include <stdint.h>
#include <string.h>
#define CLIB_IMPLEMENTATION
#include "extern/clib.h"
#define TEST_IMPLEMENTATION
#include "extern/test.h"
#include "square.h"
#include "zobrist.h"
#include <stdio.h>
#include <stdlib.h>
#include "tests.h"
#include <inttypes.h>

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

        if (strlen(move_input) != 4 && strlen(move_input) != 5) {
            printf("Invalid move format. Use 4 or 5 characters (e.g., e2e4 or h7h8Q).\n");
            continue;
        }

        char from[3], to[3], promotion;
        strncpy(from, move_input, 2);
        from[2] = '\0';
        strncpy(to, move_input + 2, 2);
        to[2] = '\0';
        promotion = move_input[4];

        if(!square_is_valid(from) || !square_is_valid(to)){
            printf("Invalid squares\n");
            continue;
        }

        square_t from_square, to_square;
        square_from_name(&from_square, from);
        square_from_name(&to_square, to);

        char piece = piece_at(&board, from_square);
        if (piece == EMPTY_SQUARE) {
            clib_ansi_clear_screen();
            PRINT_FULL(&board, NULL);
            continue;
        }

        if(!move(&board, from_square, to_square, promotion)){
            clib_ansi_clear_screen();
            PRINT_FULL(&board, NULL);
            continue;
        }


        clib_ansi_clear_screen();
        PRINT_FULL(&board, &from_square, &to_square, NULL);

        if(board.result > 0){
            printf("%s %s\n", result_message[board.result], result_score[board.result]);
            board_free(&board);
            return;
        }
    }
}

int main(int argc, char** argv){
    init_zobrist();

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
            // TEST_PAWN_IS_ENPASSANTING,
            // TEST_PAWN_CAN_ENPASSANT,
            TEST_MOVE,
            // TEST_KING_IN_CHECK,
            // TEST_BOARD_INIT_FEN,
            // TEST_INSUFFICIENT_MATERIAL,
            // TEST_MOVE_IS_VALID,
            // TEST_VALID_MOVES,
            // TEST_STALEMATE,
            // TEST_CHECKMATE,
            // TEST_IS_ATTACKED_BY,
            // TEST_ZOBRIST,
            END
        );
    }

    run("r1bqkb1r/pppp1ppp/2n2n2/4p2Q/2B1P3/8/PPPP1PPP/RNB1K1NR w KQkq - 4 4");

    board_t board;
    board_init_fen(&board, STARTING_FEN);
    MOVE(&board, "e2e4");
    MOVE(&board, "e7e5");
    MOVE(&board, "d1h5");
    MOVE(&board, "b8c6");
    MOVE(&board, "f1c4");
    MOVE(&board, "g8f6");


    char fen[128];
    fen_export(&board, fen);

    board_free(&board);

    printf("%s\n", fen);

    return 0;
}
