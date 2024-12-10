#include "bitboard.h"
#include <assert.h>

void SquareToName(char buffer[3], Square square)
{
    assert(square >= 0 && square <= 63);

    char file = 'a' + (square % 8);
    char rank = '1' + (7 - (square / 8));

    buffer[0] = file;
    buffer[1] = rank;
    buffer[2] = '\0';  // Null-terminate the string
}

Square NameToSquare(const char buffer[3])
{
    char file = buffer[0];
    char rank = buffer[1];

    int fileIndex = file - 'a';

    int rankIndex = '8' - rank;

    Square square = (rankIndex * 8) + fileIndex;

    return square;
}
