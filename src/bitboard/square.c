#include "square.h"
#include "board.h"
#include "common.h"
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

void SquareToSquareT(square_t* square, Square src)
{
    int file = (src% 8);
    int rank = (7 - (src/ 8));
    square_from_fr(square, rank, file);
}

int Rank(Square square)
{
    return square / BOARD_SIZE;
}

int File(Square square)
{
    return square % BOARD_SIZE;
}

_Bool IsSquareValid(Square square)
{
    return square >= 0 && square <= 63;
}
