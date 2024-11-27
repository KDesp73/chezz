#include "board.h"
#include "square.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define CLIB_IMPLEMENTATION
#include "extern/clib.h"

void board_init(board_t* board)
{
    // Initialize white and black pieces
    strcpy(board->grid[0], WHITE_BACK_RANK);
    strcpy(board->grid[1], WHITE_PAWN_RANK);
    for (int i = 2; i < 6; ++i) {
        memset(board->grid[i], ' ', 8);
    }
    strcpy(board->grid[6], BLACK_PAWN_RANK);
    strcpy(board->grid[7], BLACK_BACK_RANK);
}

void board_print(const board_t* board)
{
    for (int i = 7; i >= 0; --i) {
        for (int j = 0; j <= 7; ++j) {
            printf("%c ", board->grid[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

void board_print_highlight(const board_t* board, square_t** squares, size_t count)
{
    if (count == 0 || squares == NULL) {
        board_print(board);
        return;
    }

    char* yellow_bg = (char*) COLOR_BG(11);

    // Iterate through the rows of the board
    for (int i = 7; i >= 0; --i) {
        // Iterate through the columns of the board
        for (int j = 0; j <= 7; ++j) {
            // Check if the current square should be highlighted
            int highlighted = 0;
            for (size_t sc = 0; sc < count; ++sc) {
                // Adjust for 1-based rank and file
                if (i == squares[sc]->rank - 1 && j == squares[sc]->file - 1) {
                    highlighted = 1;
                    break;
                }
            }

            // Print the square, highlighted if needed
            if (highlighted) {
                printf("%s%c%s ", yellow_bg, board->grid[i][j], ANSI_RESET);
            } else {
                printf("%c ", board->grid[i][j]);
            }
        }
        printf("\n");
    }
    free(yellow_bg);
}


void board_init_fen(board_t* board, const char* fen)
{
    // TODO: Implement FEN parsing logic
}

