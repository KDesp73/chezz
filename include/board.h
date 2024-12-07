#ifndef BOARD_H
#define BOARD_H

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
#define BOARD_SIZE 8

#define STARTING_FEN "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"

typedef enum {
    ERROR_NONE = 0,
    ERROR_INVALID_MOVE,
    ERROR_EMPTY_SQUARE,
    ERROR_OBSTRUCTED_PATH,
    ERROR_INVALID_PIECE,
    ERROR_FRIENDLY_PIECE,
    ERROR_KINGS_TOUCHING,
    ERROR_INVALID_CASTLE,
    ERROR_INVALID_SQUARE,
    ERROR_PIECE_IS_PINNED,
    ERROR_CANNOT_MOVE_OPPONENTS_PIECES,
    ERROR_KING_IS_IN_CHECK,
} error_t;

static const char error_messages[][256] = {
    "OK",
    "Invalid move",
    "Empty square",
    "Obstructed path",
    "Invalid piece",
    "Attacking friendly piece",
    "Kings cannot touch",
    "Invalid castling",
    "Invalid square",
    "Piece is pinned",
    "Cannot move opponent's pieces",
    "King is in check"
};

typedef enum {
    RESULT_NONE,
    RESULT_WHITE_WON,
    RESULT_BLACK_WON,
    RESULT_STALEMATE,
    RESULT_DRAW_BY_REPETITION,
    RESULT_DRAW_DUE_TO_INSUFFICIENT_MATERIAL,
    RESULT_DRAW_DUE_TO_50_MOVE_RULE
} result_t ;

#define SCORE_COUNT 7
static const char result_score[][8] = {
    "-",
    "1-0",
    "0-1",
    "1/2-1/2",
    "1/2-1/2",
    "1/2-1/2",
    "1/2-1/2"
};
static const char result_message[][256] = {
    "No result yet",
    "White won",
    "Black won",
    "Stalemate",
    "Draw by repetition",
    "Draw due to insufficient material",
    "Draw due to 50 move rule"
};

typedef struct {
    char grid[8][8];
    _Bool turn;
    char enpassant_square[2 + 1]; // +1 for null terminator
    uint8_t castling_rights;
    size_t halfmove;
    size_t fullmove;
    error_t error;
    result_t result;
    hash_table_t history;
} board_t;

enum {
    CHECK_WHITE_KING = 0b01,
    CHECK_BLACK_KING = 0b10,
};

enum {
    CASTLE_WHITE_KINGSIDE = 0b0001,
    CASTLE_WHITE_QUEENSIDE = 0b0010,
    CASTLE_BLACK_KINGSIDE = 0b0100,
    CASTLE_BLACK_QUEENSIDE = 0b1000,
};

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
