#include "board.h"
#include "extern/clib.h"
#include "tests.h"
#include "extern/test.h"
#include <stdint.h>
#include <string.h>

int test_board_init_fen(const char* fen, _Bool turn, uint8_t castling_rights, char enpassant_square[3], size_t halfmove, size_t fullmove)
{
    board_t board;
    board_init_fen(&board, fen);

    if(turn != board.turn){
        FAIL("For fen %s. Expected turn %d. Found %d", fen, turn, !turn);
        return 0;
    }

    if(castling_rights != board.castling_rights){
        FAIL("For fen %s. Expected castling rights %d. Found %d", fen, castling_rights, board.castling_rights);
        return 0;
    }

    if(strcmp(enpassant_square, board.enpassant_square)){
        FAIL("For fen %s. Expected enpassant square %s. Found %s", fen, enpassant_square, board.enpassant_square);
        return 0;
    }

    if(halfmove != board.halfmove){
        FAIL("For fen %s. Expected halfmove %zu. Found %zu", fen, halfmove, board.halfmove);
        return 0;
    }

    if(fullmove != board.fullmove){
        FAIL("For fen %s. Expected fullmove %zu. Found %zu", fen, fullmove, board.fullmove);
        return 0;
    }

    SUCC("Passed for fen %s", fen);
    return 1;
}
