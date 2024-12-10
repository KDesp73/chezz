#include "board.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "common.h"
#include "notation.h"

/**
 * Parses a FEN string and initializes the Board structure.
 */
void BoardInitFen(Board *board, const char *fen)
{
    if(fen == NULL) {
        BoardInitFen(board, STARTING_FEN);
        return;
    }

    FenImport(board, fen);

    board->state.error = 0;
    board->state.result = RESULT_NONE;
    init_hash_table(&board->state.history, 1000, fen);
}

/**
 * Prints a Board structure for debugging purposes.
 */
void BoardPrint(Board board)
{
    for (int rank = 7; rank >= 0; rank--) {
        for (int file = 0; file < 8; file++) {
            int square = rank * 8 + file;
            char piece = '.';
            for (int i = 0; i < PIECE_TYPE_COUNT; i++) {
                if (board.bitboards[i] & (1ULL << square)) {
                    piece = "pnbrqkPNBRQK"[i];
                    break;
                }
            }
            printf("%c ", piece);
        }
        printf("\n");
    }
    printf("\n");
    printf("Active color: %c\n", board.state.turn ? 'w' : 'b');
    printf("Castling rights: %x\n", board.state.castling_rights);
    printf("En passant square: %d\n", board.enpassant_square);
    printf("Halfmove clock: %zu\n", board.state.halfmove);
    printf("Fullmove number: %zu\n", board.state.fullmove);
}

