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

int test_valid_moves(const char* fen, const char* square, const char* first, ...)
{
    board_t board;
    board_init_fen(&board, fen);
    
    size_t moves_count = 0;
    square_t** moves = valid_moves(&board, square_from_name(square), &moves_count);
    printf("Found: ");
    for(size_t i = 0; i < moves_count; i++){
        printf("%s ", moves[i]->name);
    }
    printf("\n");

    // Collect expected moves from variadic arguments
    size_t expected_count = 0;
    square_t** expected_moves = malloc(64 * sizeof(square_t*)); // Max 64 moves, adjust if needed

    va_list args;
    va_start(args, first);

    const char* current = first;
    printf("Expected: ");
    while (current != NULL) {
        printf("%s ", current);
        expected_moves[expected_count++] = square_from_name(current);
        current = va_arg(args, const char*);
    }
    printf("\n");
    va_end(args);

    if (moves_count != expected_count) {
        FAIL("Expected %zu moves, but got %zu moves.", expected_count, moves_count);
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
            FAIL("Expected move %s not found.", expected_moves[i]->name);
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
            FAIL("Unexpected move %s found.\n", moves[j]->name);
            goto cleanup;
        }
    }

    // Test passed
    for (size_t i = 0; i < expected_count; i++) {
        square_free(&expected_moves[i]);
    }
    free(expected_moves);

    SUCC("Passed for fen %s, piece %s", fen, square);
    return 1;

cleanup:
    for (size_t i = 0; i < expected_count; i++) {
        square_free(&expected_moves[i]);
    }
    free(expected_moves);
    return 0;
}
