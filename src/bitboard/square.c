#include "bitboard.h"
#include <assert.h>

void SquareName(char buffer[3], Square square)
{
    assert(square >= 0 && square <= 63);

    char file = 'a' + (square % 8);
    char rank = '1' + (7 - (square / 8));

    buffer[0] = file;
    buffer[1] = rank;
    buffer[2] = '\0';  // Null-terminate the string
}

