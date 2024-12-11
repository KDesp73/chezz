#include "board.h"
#include "move.h"
#include "square.h"
#include <stdio.h>

Move MoveEncode(Square from, Square to, uint8_t promotion, uint8_t flag)
{
    return (from & 0x3F) | 
           ((to & 0x3F) << 6) | 
           ((promotion & 0xF) << 12) | 
           ((flag & 0x7) << 16);
}

void MoveDecode(Move move, Square* from, Square* to, uint8_t* promotion, uint8_t* flag)
{
    *from = move & 0x3F;
    *to = (move >> 6) & 0x3F;
    *promotion = (move >> 12) & 0xF;
    *flag = (move >> 16) & 0x7;
}

void MoveApply(Board board, Move move, uint8_t color)
{
    Square from, to;
    uint8_t promotion, flags;
    MoveDecode(move, &from, &to, &promotion, &flags);

    uint64_t from_bb = 1ULL << from;
    uint64_t to_bb = 1ULL << to;

    // Find the piece that moved
    for (int piece = 0; piece < 6; piece++) {
        if (board.bitboards[color * 6 + piece] & from_bb) {
            // Move the piece
            board.bitboards[color * 6 + piece] ^= from_bb; // Remove from source
            if (promotion) {
                // Add promoted piece
                board.bitboards[color * 6 + promotion - 1] |= to_bb;
            } else {
                // Move to destination
                board.bitboards[color * 6 + piece] |= to_bb;
            }
            break;
        }
    }

    // Handle captures
    int opponent = 1 - color;
    for (int piece = 0; piece < 6; piece++) {
        if (board.bitboards[opponent * 6 + piece] & to_bb) {
            board.bitboards[opponent * 6 + piece] ^= to_bb;
            break;
        }
    }

    // Special moves
    if (flags == 1) {
        // Handle castling
        if (to == 6) { // Kingside castling
            board.bitboards[color * 6 + 0] ^= (1ULL << 7) | (1ULL << 5); // Move rook
        } else if (to == 2) { // Queenside castling
            board.bitboards[color * 6 + 0] ^= (1ULL << 0) | (1ULL << 3); // Move rook
        }
    } else if (flags == 2) {
        // Handle en passant
        board.bitboards[opponent * 6 + 5] ^= (1ULL << (to - (color ? 8 : -8)));
    }
}

void MovePrint(Move move)
{
    Square from, to;
    uint8_t promotion, flags;
    MoveDecode(move, &from, &to, &promotion, &flags);

    char square_from[3], square_to[3];
    SquareToName(square_from, from);
    SquareToName(square_to, to);
    printf("Move: %s -> %s, Promotion: %d, Flags: %d\n",
            square_from, square_to,
            promotion, flags);
}

_Bool MoveIsValid(Board board, Move move, uint8_t color)
{
    Square from, to;
    uint8_t promotion, flags;
    MoveDecode(move, &from, &to, &promotion, &flags);
    uint64_t from_bb = 1ULL << from;
    uint64_t to_bb = 1ULL << to;

    for (int piece = 0; piece < 6; piece++) {
        if (board.bitboards[color * 6 + piece] & from_bb) {
            return 1;
        }
    }

    return 0;
}

char PromotionToChar(uint8_t promotion)
{
    switch (promotion) {
    case PROMOTION_QUEEN: return 'Q';
    case PROMOTION_ROOK: return 'R';
    case PROMOTION_BISHOP: return 'B';
    case PROMOTION_KNIGHT: return 'N';
    case PROMOTION_NONE: 
    default:
          return '\0';
    }
}

uint8_t CharToPromotion(char promotion)
{
    switch (promotion) {
    case 'Q': return PROMOTION_QUEEN;
    case 'R': return PROMOTION_ROOK;
    case 'B': return PROMOTION_BISHOP;
    case 'N': return PROMOTION_KNIGHT;
    default:
          return PROMOTION_NONE;
    }
}
