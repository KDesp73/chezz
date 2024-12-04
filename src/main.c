#include "board.h"
#include "move.h"
#include "ui.h"
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

int main(int argc, char** argv){
    init_zobrist();

    if(argc == 2 && STREQ(argv[1], "test")){
        return !test(
            TEST_SQUARE_FROM_NAME,
            TEST_PAWN_MOVE,
            TEST_ROOK_MOVE,
            TEST_BISHOP_MOVE,
            TEST_QUEEN_MOVE,
            TEST_KNIGHT_MOVE,
            TEST_KING_MOVE,
            TEST_IS_PINNED,
            TEST_PAWN_IS_ENPASSANTING,
            TEST_PAWN_CAN_ENPASSANT,
            TEST_MOVE,
            TEST_KING_IN_CHECK,
            TEST_BOARD_INIT_FEN,
            TEST_INSUFFICIENT_MATERIAL,
            TEST_MOVE_IS_VALID,
            TEST_VALID_MOVES,
            TEST_STALEMATE,
            TEST_CHECKMATE,
            TEST_IS_ATTACKED_BY,
            // TEST_ZOBRIST,
            END
        );
    }

    board_t board;
    board_init_fen(&board, "rnbqkbnr/pppp1ppp/8/4p3/4P3/8/PPPP1PPP/RNBQKBNR w KQkq - 0 1");

    square_t square;
    square_from_name(&square, "f1");

    size_t count;
    square_t** valid = valid_moves(&board, square, &count);

    tui_board_print_squares(&board, FULL_CONFIG, valid, count);


    squares_free(&valid, count);
    board_free(&board);

    return 0;
}
