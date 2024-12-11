#include "board.h"
#include "square.h"
#include <stdio.h>

void Uint32Print(uint32_t value)
{
    for (int i = 31; i >= 0; i--) {
        printf("%d", (value >> i) & 1);
        if (i % 4 == 0) {
            printf(" ");
        }
    }
    printf("\n");
}

void Uint64Print(uint64_t value)
{
    for (int i = 63; i >= 0; i--) {
        printf("%lu", (value >> i) & 1);
        if (i % 8 == 0) {
            printf(" ");
        }
    }
    printf("\n");
}

void BitboardPrint(Board board, int index)
{
    const char* yellow_bg = "\033[48;5;214m"; // Background yellow color
    const char* reset = "\033[0m";             // Reset color formatting

    const char* padding = "   ";
    Bitboard highlight_bitboard = board.bitboards[index];

    printf("%s┌───┬───┬───┬───┬───┬───┬───┬───┐\n", padding);

    for (int rank = 7; rank >= 0; rank--){

        printf(" %d ", rank + 1);

        for (int file = 0; file <= 7; ++file) {
            printf("│");

            char piece = ' '; // Default empty square
            int square_index = rank * 8 + file;

            // Check if this square is occupied by a piece
            for (int i = 0; i < PIECE_TYPE_COUNT; ++i) {
                if (board.bitboards[i] & (1ULL << square_index)) {
                    piece = "pnbrqkPNBRQK"[i];
                    break;
                }
            }

            // Check if the square is highlighted in the Bitboard
            int highlighted = (highlight_bitboard & (1ULL << square_index)) != 0;

            if (highlighted) {
                printf("%s %c %s", yellow_bg, piece, reset);
            } else {
                printf(" %c ", piece);
            }
        }
        printf("│\n");

        if (rank != 0) {
            printf("%s├───┼───┼───┼───┼───┼───┼───┼───┤\n", padding);
        }
    }
    printf("%s└───┴───┴───┴───┴───┴───┴───┴───┘\n", padding);

    printf("%s", padding);
    for (int file = 0; file < 8; ++file) {
        char label = 'a' + file;
        printf("  %c ", label);
    }
    printf("\n");
}

