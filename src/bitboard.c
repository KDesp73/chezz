#include "bitboard.h"
#include "board.h"
#include "square.h"
#include <stdio.h>

move_t encode_move(int from, int to, int promotion, int flags)
{
    return (from & 0x3F) | 
           ((to & 0x3F) << 6) | 
           ((promotion & 0xF) << 12) | 
           ((flags & 0x3) << 16);
}

void decode_move(move_t move, int* from, int* to, int* promotion, int* flags)
{
    *from = move & 0x3F;
    *to = (move >> 6) & 0x3F;
    *promotion = (move >> 12) & 0xF;
    *flags = (move >> 16) & 0x3;
}

void apply_move(uint64_t* bitboards, move_t move, int color)
{
    int from, to, promotion, flags;
    decode_move(move, &from, &to, &promotion, &flags);

    uint64_t from_bb = 1ULL << from;
    uint64_t to_bb = 1ULL << to;

    // Find the piece that moved
    for (int piece = 0; piece < 6; piece++) {
        if (bitboards[color * 6 + piece] & from_bb) {
            // Move the piece
            bitboards[color * 6 + piece] ^= from_bb; // Remove from source
            if (promotion) {
                // Add promoted piece
                bitboards[color * 6 + promotion - 1] |= to_bb;
            } else {
                // Move to destination
                bitboards[color * 6 + piece] |= to_bb;
            }
            break;
        }
    }

    // Handle captures
    int opponent = 1 - color;
    for (int piece = 0; piece < 6; piece++) {
        if (bitboards[opponent * 6 + piece] & to_bb) {
            bitboards[opponent * 6 + piece] ^= to_bb;
            break;
        }
    }

    // Special moves
    if (flags == 1) {
        // Handle castling
        if (to == 6) { // Kingside castling
            bitboards[color * 6 + 0] ^= (1ULL << 7) | (1ULL << 5); // Move rook
        } else if (to == 2) { // Queenside castling
            bitboards[color * 6 + 0] ^= (1ULL << 0) | (1ULL << 3); // Move rook
        }
    } else if (flags == 2) {
        // Handle en passant
        bitboards[opponent * 6 + 5] ^= (1ULL << (to - (color ? 8 : -8)));
    }
}

void print_move(move_t move)
{
    int from, to, promotion, flags;
    decode_move(move, &from, &to, &promotion, &flags);
    printf("Move: %c%d -> %c%d, Promotion: %d, Flags: %d\n",
           'a' + (from % 8), 1 + (from / 8),
           'a' + (to % 8), 1 + (to / 8),
           promotion, flags);
}

_Bool is_valid_move(uint64_t* bitboards, move_t move, int color)
{
    int from, to, promotion, flags;
    decode_move(move, &from, &to, &promotion, &flags);
    uint64_t from_bb = 1ULL << from;
    uint64_t to_bb = 1ULL << to;

    // Ensure the source square contains a piece of the current color
    for (int piece = 0; piece < 6; piece++) {
        if (bitboards[color * 6 + piece] & from_bb) {
            return 1;
        }
    }

    return 0;
}

move_t squares_to_move(square_t from, square_t to, int promotion, int flags)
{
    // Encode the 'rank' and 'file' as a 6-bit index (0-63)
    size_t from_index = (from.rank * BOARD_SIZE) + from.file; // Calculate 0-63 index from rank/file
    size_t to_index = (to.rank * BOARD_SIZE) + to.file;       // Calculate 0-63 index from rank/file

    // Build move_t by packing from_index, to_index, promotion, and flags
    return (from_index & 0x3F) | 
           ((to_index & 0x3F) << 6) | 
           ((promotion & 0xF) << 12) | 
           ((flags & 0x3) << 16);
}

void move_to_squares(move_t move, square_t* from, square_t* to, int* promotion, int* flags)
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
