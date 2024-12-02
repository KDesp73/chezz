#ifndef MOVE_H
#define MOVE_H

#include "board.h"
void move(board_t* board, square_t* from, square_t* to);

_Bool piece_can_move(board_t* board, const square_t* piece, const square_t* target);
_Bool pawn_can_move(board_t* board, const square_t* piece, const square_t* target);
_Bool pawn_can_attack(board_t* board, const square_t* piece, const square_t* target);
_Bool rook_can_move(board_t* board, const square_t* piece, const square_t* target);
_Bool knight_can_move(board_t* board, const square_t* piece, const square_t* target);
_Bool bishop_can_move(board_t* board, const square_t* piece, const square_t* target);
_Bool queen_can_move(board_t* board, const square_t* piece, const square_t* target);
_Bool king_can_move(board_t* board, const square_t* piece, const square_t* target);

square_t** valid_moves(board_t* board, const square_t* piece, size_t* count);
_Bool king_is_castling(const board_t* board, const square_t* from, const square_t* to);

_Bool can_castle(board_t* board, square_t* from, square_t* to);
void castle(board_t* board, square_t* from, square_t* to);

_Bool move_is_valid(const board_t* board, const square_t* from, const square_t* to);

#endif // MOVE_H
