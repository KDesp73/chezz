#ifndef MOVE_H
#define MOVE_H

#include "board.h"

void move_freely(board_t* board, square_t from, square_t to);
_Bool move(board_t* board, square_t from, square_t to, char promotion);

_Bool piece_can_move(board_t* board, square_t piece, square_t target);
_Bool pawn_can_move(board_t* board, square_t piece, square_t target);
_Bool pawn_can_attack(board_t* board, square_t piece, square_t target, _Bool strict);
_Bool rook_can_move(board_t* board, square_t piece, square_t target);
_Bool knight_can_move(board_t* board, square_t piece, square_t target);
_Bool bishop_can_move(board_t* board, square_t piece, square_t target);
_Bool queen_can_move(board_t* board, square_t piece, square_t target);
_Bool king_can_move(board_t* board, square_t piece, square_t target);

square_t** valid_moves(board_t* board, square_t piece, size_t* count);
_Bool move_is_valid(board_t* board, square_t from, square_t to);
_Bool attack_is_valid(board_t* board, square_t from, square_t to, _Bool strict);

_Bool king_is_castling(const board_t* board, square_t from, square_t to);
_Bool king_can_castle(board_t* board, square_t from, square_t to);
void king_castle(board_t* board, square_t from, square_t to);

_Bool pawn_is_enpassanting(const board_t* board, square_t from, square_t to);
_Bool pawn_can_enpassant(const board_t* board, square_t from, square_t to);
void pawn_enpassant(board_t* board, square_t from, square_t to);
_Bool pawn_is_promoting(const board_t* board, square_t from, square_t to);
_Bool pawn_promote(board_t* board, square_t from, square_t to, char promotion);


/*--------------------------------------.
| Bitboard move representation.         |
| Moves are 32-bit integers             |
| where bits:                           |
| - 0-5 represent the source            |
| - 6-11 represent the destination      |
| - 12-15 represent the promotion piece |
| - 16-21 represent the flags           |
`--------------------------------------*/

typedef uint32_t Move;

#define KNIGHT_OFFSETS_COUNT 8
const static int KNIGHT_OFFSETS[] = {
    -17, -15, -10, -6, 6, 10, 15, 17
};
#define BISHOP_OFFSETS_COUNT 4
const static int BISHOP_OFFSETS[] = {
    -9, -7, 7, 9
};
#define ROOK_OFFSETS_COUNT 4
const static int ROOK_OFFSETS[] = {
    -8, -1, 1, 8
};
#define KING_OFFSETS_COUNT 8
const static int KING_OFFSETS[] = {
    -9, -8, -7, -1, 1, 7, 8, 9
};


_Bool MoveIsValid(const Board* board, Move move, uint8_t color);
Move MoveEncode(Square from, Square to, uint8_t promotion, uint8_t flag);
Move MoveEncodeNames(const char* from, const char* to, uint8_t promotion, uint8_t flag);
void MoveDecode(Move move, Square* from, Square* to, uint8_t* promotion, uint8_t* flag);
_Bool MoveMake(Board* board, Move move);
void MoveFreely(Board* board, Move move, uint8_t color);
void MovePrint(Move move);

Square GetFrom(Move move);
Square GetTo(Move move);
uint8_t GetPromotion(Move move);
uint8_t GetFlag(Move move);

// Piece Movement Functions
_Bool PieceCanMove(const Board* board, Square from, Square to, Flag* flag);
_Bool CanMovePawn(const Board* board, Square from, Square to, uint8_t color, Flag* flag);
_Bool CanMoveKnight(const Board* board, Square from, Square to, uint8_t color);
_Bool CanMoveBishop(const Board* board, Square from, Square to, uint8_t color);
_Bool CanMoveRook(const Board* board, Square from, Square to, uint8_t color);
_Bool CanMoveQueen(const Board* board, Square from, Square to, uint8_t color);
_Bool CanMoveKing(const Board* board, Square from, Square to, uint8_t color, Flag* flag);

// Move List Generation
void GenerateLegalMoves(const Board* board, uint8_t color, Move* moves, size_t* move_count);
void GenerateLegelMoves(const Board* board, Square piece, Move* moves, size_t* move_count);
Bitboard GeneratePawnMoves(const Board* board, Square from, uint8_t color);
Bitboard GenerateKingMoves(const Board* board, Square from);
Bitboard GenerateQueenMoves(const Board* board, Square from);
Bitboard GenerateBishopMoves(const Board* board, Square from);
Bitboard GenerateRookMoves(const Board* board, Square from);
Bitboard GenerateKnightMoves(const Board* board, Square from);

// Adapters
Move SquaresToMove(square_t from, square_t to, uint8_t promotion, uint8_t flags);
void MoveToSquares(Move move, square_t* from, square_t* to, uint8_t* promotion, uint8_t* flags);

void UpdateHalfmove(Board* board, Move move, size_t piece_count_before, size_t piece_count_after, char piece);
uint8_t UpdateCastlingRights(Board* board, Square from);
Square UpdateEnpassantSquare(Board* board, Move move);

#endif // MOVE_H
