#ifndef BITBOARD_H
#define BITBOARD_H

/* Move structure
Bits 0-5: Source square (0-63)
Bits 6-11: Destination square (0-63)
Bits 12-15: Promotion piece (0: None, 1: Queen, 2: Rook, 3: Bishop, 4: Knight)
Bits 16-17: Flags (0: Normal, 1: Castling, 2: En Passant, etc.)
*/

#include "board.h"
#include "common.h"
#include "square.h"
#include <stdint.h>


typedef uint32_t Move;
typedef uint64_t Bitboard;
typedef int Square;

#define PIECE_TYPE_COUNT 12

typedef struct {
    Bitboard bitboards[PIECE_TYPE_COUNT];
    int enpassant_square;
    state_t state;
} Board;

enum {
    INDEX_BLACK_PAWN,
    INDEX_BLACK_KNIGHT,
    INDEX_BLACK_BISHOP,
    INDEX_BLACK_ROOK,
    INDEX_BLACK_QUEEN,
    INDEX_BLACK_KING,
    INDEX_WHITE_PAWN,
    INDEX_WHITE_KNIGHT,
    INDEX_WHITE_BISHOP,
    INDEX_WHITE_ROOK,
    INDEX_WHITE_QUEEN,
    INDEX_WHITE_KING,
};

_Bool MoveIsValid(Board board, Move move, int color);
Move MoveEncode(Square from, Square to, int promotion, int flags);
void MoveApply(Board board, Move move, int color);
void MoveDecode(Move move, Square* from, Square* to, int* promotion, int* flags);
void MovePrint(Move move);

// Adaptors
Move SquaresToMove(square_t from, square_t to, int promotion, int flags);
void MoveToSquares(Move move, square_t* from, square_t* to, int* promotion, int* flags);

#endif // BITBOARD_H
