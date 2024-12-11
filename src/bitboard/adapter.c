#include "move.h"
#include <stdio.h>

Move SquaresToMove(square_t from, square_t to, uint8_t promotion, uint8_t flags)
{
    // Encode the 'rank' and 'file' as a 6-bit index (0-63)
    size_t from_index = (from.rank * BOARD_SIZE) + from.file; // Calculate 0-63 index from rank/file
    size_t to_index = (to.rank * BOARD_SIZE) + to.file;       // Calculate 0-63 index from rank/file

    // Build Move by packing from_index, to_index, promotion, and flags
    return (from_index & 0x3F) | 
           ((to_index & 0x3F) << 6) | 
           ((promotion & 0xF) << 12) | 
           ((flags & 0x3) << 16);
}

void MoveToSquares(Move move, square_t* from, square_t* to, uint8_t* promotion, uint8_t* flags)
{
    size_t from_index = move & 0x3F;               // Extract bits 0-5 for from index
    size_t to_index = (move >> 6) & 0x3F;          // Extract bits 6-11 for to index
    *promotion = (move >> 12) & 0xF;               // Extract bits 12-15 for promotion
    *flags = (move >> 16) & 0x3;                   // Extract bits 16-17 for flags

    // Convert the indices back to rank/file for both from and to squares
    from->rank = from_index / BOARD_SIZE;          // Rank = index / BOARD_SIZE
    from->file = from_index % BOARD_SIZE;          // File = index % BOARD_SIZE
    to->rank = to_index / BOARD_SIZE;              // Rank = index / BOARD_SIZE
    to->file = to_index % BOARD_SIZE;              // File = index % BOARD_SIZE

    // Optional: Fill in `name` or other fields if needed
    snprintf(from->name, sizeof(from->name), "%c%d", 'A' + (int)from->file, BOARD_SIZE - (int)from->rank);
    snprintf(to->name, sizeof(to->name), "%c%d", 'A' + (int)to->file, BOARD_SIZE - (int)to->rank);
}

