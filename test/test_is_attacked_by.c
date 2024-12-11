#include "bitboard-tests.h"
#include "board.h"
#include "extern/clib.h"
#include "piece.h"
#include "square.h"
#include "grid-tests.h"
#include "extern/test.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// #define PIECES

int test_is_attacked_by(const char* fen, const char* square, int attacked_by, char* first, ...)
{
    board_t board;
    board_init_fen(&board, fen);

    square_t sqr;
    square_from_name(&sqr, square);

    size_t moves_count = 0;
    square_t** moves = square_is_attacked_by(&board, sqr, attacked_by, &moves_count);

#ifdef PIECES
    printf("Found: ");
    for (size_t i = 0; i < moves_count; i++) {
        printf("%s ", moves[i]->name);
    }
    printf("\n");
#endif

    size_t expected_count = 0;
    square_t** expected_moves = malloc(64 * sizeof(square_t*)); // Max 64 moves

    va_list args;
    va_start(args, first);

    const char* current = first;
#ifdef PIECES
    printf("Expected: ");
#endif
    while (current != NULL) {
#ifdef PIECES
        printf("%s ", current);
#endif
        expected_moves[expected_count] = square_new_name(current);
        expected_count++;
        current = va_arg(args, const char*);
    }
#ifdef PIECES
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


int TestIsAttackedBy(const char* fen, const char* square, int attacked_by, char* first, ...)
{
    Board board;
    BoardInitFen(&board, fen);

    Square sqr = NameToSquare(square);

    size_t moves_count = 0;
    Square* moves = SquareIsAttackedBy(&board, sqr, attacked_by, &moves_count);

#ifdef PIECES
    printf("Found: ");
    for (size_t i = 0; i < moves_count; i++) {
        char name[3];
        SquareToName(name, moves[i]);
        printf("%s ", name);
    }
    printf("\n");
#endif

    size_t expected_count = 0;
    Square* expected_moves = malloc(64 * sizeof(Square)); // Max 64 moves

    va_list args;
    va_start(args, first);

    const char* current = first;
#ifdef PIECES
    printf("Expected: ");
#endif
    while (current != NULL) {
#ifdef PIECES
        printf("%s ", current);
#endif
        expected_moves[expected_count] = SquareFromName(current);
        expected_count++;
        current = va_arg(args, const char*);
    }
#ifdef PIECES
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
            if (expected_moves[i] == moves[i]){
                found = 1;
                break;
            }
        }
        if (!found) {
            char name[3];
            SquareToName(name, expected_moves[i]);
            FAIL("For fen %s square %s. Expected move %s not found.", fen, square, name);
            goto cleanup;
        }
    }

    // Check for unexpected moves
    for (size_t j = 0; j < moves_count; j++) {
        int found = 0;
        for (size_t i = 0; i < expected_count; i++) {
            if (moves[j] == expected_moves[i]){
                found = 1;
                break;
            }
        }
        if (!found) {
            char name[3];
            SquareToName(name, expected_moves[j]);
            FAIL("For fen %s square %s. Unexpected move %s found.", fen, square, name);
            goto cleanup;
        }
    }


    SUCC("Passed for fen %s, piece %s", fen, square);
    return 1;

cleanup:
    return 0;
}
