#ifndef BOARD_H
#define BOARD_H

#include "common.h"
#include "hashing.h"
#include "square.h"
#include <stdint.h>

#define BLACK_ROOK 'r'
#define BLACK_KNIGHT 'n'
#define BLACK_BISHOP 'b'
#define BLACK_KING 'k'
#define BLACK_QUEEN 'q'
#define BLACK_PAWN 'p'

#define WHITE_ROOK 'R'
#define WHITE_KNIGHT 'N'
#define WHITE_BISHOP 'B'
#define WHITE_KING 'K'
#define WHITE_QUEEN 'Q'
#define WHITE_PAWN 'P'

#define EMPTY_SQUARE ' '

#define STARTING_FEN "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"

typedef struct {
    char grid[8][8];
    state_t state;
    char enpassant_square[2 + 1]; // +1 for null terminator
} board_t;

void board_init_fen(board_t* board, const char* fen);
void board_init_board(board_t* board, board_t src);
void board_free(board_t* board);

int has_castling_rights(const board_t* board, uint8_t castling_right);
void revoke_castling_rights(board_t* board, uint8_t castling_rights);

_Bool square_is_attacked(const board_t* board, square_t square, int attacked_by);
_Bool square_is_attacked_coords(const board_t *board, int y, int x, int attacked_by);
_Bool square_is_attacked_fr(const board_t *board, int rank, int file, int attacked_by);
square_t** square_is_attacked_by(const board_t* board, square_t square, int attacked_by, size_t* count);
square_t** square_is_accessible_by(const board_t* board, square_t square, char piece, size_t* count);
square_t** attack_path_to_king(board_t* board, square_t king, square_t attacker, size_t* path_count);

void find_king(square_t* square, const board_t* board, int color);

_Bool in_check(const board_t* board, int color);
#define IN_CHECK(board, color) \
    in_check(board, color)

char* update_enpassant_square(board_t* board, square_t from, square_t to);
uint8_t update_castling_rights(board_t* board, square_t from);
void update_halfmove(board_t* board, square_t from, square_t to, size_t piece_count_before, size_t piece_count_after, char piece);

size_t number_of_pieces(const board_t* board, int color);

_Bool is_checkmate(board_t* board);
_Bool is_stalemate(board_t* board);
_Bool is_insufficient_material(board_t* board);
_Bool is_threefold_repetition(board_t* board);

#endif // BOARD_H
