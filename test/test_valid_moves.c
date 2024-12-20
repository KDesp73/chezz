#include "board.h"
#include "extern/clib.h"
#include "move.h"
#include "square.h"
#include "tests.h"
#include "extern/test.h"
#include <stdlib.h>

#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#define MOVES

int test_valid_moves(const char* fen, const char* square, const char* first, ...)
{
    board_t board;
    board_init_fen(&board, fen);

    square_t sqr;
    square_from_name(&sqr, square);

    size_t moves_count = 0;
    square_t** moves = valid_moves(&board, sqr, &moves_count);
#ifdef MOVES
    printf("Found: ");
    for (size_t i = 0; i < moves_count; i++) {
        printf("%s ", moves[i]->name);
    }
    printf("\n");
#endif

    // Collect expected moves from variadic arguments
    size_t expected_count = 0;
    square_t** expected_moves = malloc(64 * sizeof(square_t*)); // Max 64 moves

    va_list args;
    va_start(args, first);

    const char* current = first;
#ifdef MOVES
    printf("Expected: ");
#endif
    while (current != NULL) {
#ifdef MOVES
        printf("%s ", current);
#endif
        expected_moves[expected_count] = malloc(sizeof(square_t));
        square_from_name(expected_moves[expected_count], current);
        expected_count++;
        current = va_arg(args, const char*);
    }
#ifdef MOVES
    printf("\n");
#endif
    va_end(args);

    if (moves_count != expected_count) {
        FAIL("For fen %s square %s. Expected %zu moves, but got %zu moves.", fen, square, expected_count, moves_count);
        goto cleanup;
    }

    // Check if all expected moves are present in the actual moves
    for (size_t i = 0; i < expected_count; i++) {
        int found = 0;
        for (size_t j = 0; j < moves_count; j++) {
            if (expected_moves[i]->rank == moves[j]->rank &&
                expected_moves[i]->file == moves[j]->file) {
                found = 1;
                break;
            }
        }
        if (!found) {
            FAIL("For fen %s square %s. Expected move %s not found.", fen, square, expected_moves[i]->name);
            goto cleanup;
        }
    }

    // Check for unexpected moves
    for (size_t j = 0; j < moves_count; j++) {
        int found = 0;
        for (size_t i = 0; i < expected_count; i++) {
            if (moves[j]->rank == expected_moves[i]->rank &&
                moves[j]->file == expected_moves[i]->file) {
                found = 1;
                break;
            }
        }
        if (!found) {
            FAIL("For fen %s square %s. Unexpected move %s found.", fen, square, moves[j]->name);
            goto cleanup;
        }
    }


    SUCC("Passed for fen %s, piece %s", fen, square);
    return 1;

cleanup:
    return 0;
}
