#ifndef MOVE_H
#define MOVE_H

#include "board.h"
#include "notation.h"
void move_freely(board_t* board, square_t from, square_t to);
_Bool move(board_t* board, square_t from, square_t to, char promotion);

_Bool move_name(board_t* board, game_t* game, const char* move_str);
#define MOVE(board, game, move) \
        move_name(board, game, move)

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


#endif // MOVE_H
