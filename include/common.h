#ifndef COMMON_H
#define COMMON_H

#include "hashing.h"
#include <stddef.h>
#include <stdint.h>

#define BOARD_SIZE 8

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
    "*",
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
    _Bool turn;
    uint8_t castling_rights;
    size_t halfmove;
    size_t fullmove;
    error_t error;
    result_t result;
    hash_table_t history;
} state_t;

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

#define board_free(board) \
    free_hash_table(&((board)->state.history))

int has_castling_rights(state_t state, uint8_t castling_right);
void revoke_castling_rights(state_t* state, uint8_t castling_rights);

#endif // COMMON_H
