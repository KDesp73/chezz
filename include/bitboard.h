#ifndef BITBOARD_H
#define BITBOARD_H

#include "common.h"
#include "square.h"
#include <stdint.h>

#define PIECE_TYPE_COUNT 12

/* Move structure
Bits 0-5: Source square (0-63)
Bits 6-11: Destination square (0-63)
Bits 12-15: Promotion piece (0: None, 1: Queen, 2: Rook, 3: Bishop, 4: Knight)
Bits 16-21: Flags (0: Normal, 1: Castling, 2: En Passant, etc.)
*/
typedef uint32_t Move;
typedef uint64_t Bitboard;
typedef uint8_t Square;

typedef struct {
    Bitboard bitboards[PIECE_TYPE_COUNT];
    Square enpassant_square;
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

typedef enum {
    FLAG_NORMAL = 0,
    FLAG_CASTLING,
    FLAG_ENPASSANT,
    FLAG_PAWN_DOUBLE_MOVE,
    FLAG_PROMOTION,
    FLAG_PROMOTION_WITH_CAPTURE,
} Flag;

typedef enum {
    PROMOTION_NONE = 0,
    PROMOTION_QUEEN,
    PROMOTION_ROOK,
    PROMOTION_BISHOP,
    PROMOTION_KNIGHT
} Promotion;

void Uint32Print(uint32_t value);

_Bool MoveIsValid(Board board, Move move, uint8_t color);
Move MoveEncode(Square from, Square to, uint8_t promotion, uint8_t flag);
void MoveDecode(Move move, Square* from, Square* to, uint8_t* promotion, uint8_t* flag);
void MoveApply(Board board, Move move, uint8_t color);
void MovePrint(Move move);

void SquareName(char buffer[3], Square square);

// Adapters
Move SquaresToMove(square_t from, square_t to, uint8_t promotion, uint8_t flags);
void MoveToSquares(Move move, square_t* from, square_t* to, uint8_t* promotion, uint8_t* flags);

#endif // BITBOARD_H
