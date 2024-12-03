#ifndef BITBOARD_H
#define BITBOARD_H

/*
Bits 0-5: Source square (0-63)
Bits 6-11: Destination square (0-63)
Bits 12-15: Promotion piece (0: None, 1: Queen, 2: Rook, 3: Bishop, 4: Knight)
Bits 16-17: Flags (0: Normal, 1: Castling, 2: En Passant, etc.)
*/

#include "square.h"
#include <stdint.h>
typedef uint32_t move_t;

_Bool is_valid_move(uint64_t* bitboards, move_t move, int color);
move_t encode_move(int from, int to, int promotion, int flags);
void apply_move(uint64_t* bitboards, move_t move, int color);
void decode_move(move_t move, int* from, int* to, int* promotion, int* flags);
void print_move(move_t move);
move_t squares_to_move(square_t from, square_t to, int promotion, int flags);
void move_to_squares(move_t move, square_t* from, square_t* to, int* promotion, int* flags);

#endif // BITBOARD_H
