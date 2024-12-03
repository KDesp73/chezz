#ifndef ZOBRIST_H
#define ZOBRIST_H

#include "board.h"
#include <stdint.h>
uint64_t calculate_zobrist_hash(board_t* board);

#define BOARD_SIZE 8
#define PIECE_TYPES 12
#define CASTLING_OPTIONS 4

static uint64_t zobrist_table[PIECE_TYPES][BOARD_SIZE][BOARD_SIZE];
static uint64_t zobrist_castling[CASTLING_OPTIONS];
static uint64_t zobrist_en_passant[BOARD_SIZE];

void init_zobrist();
uint64_t calculate_zobrist_hash(board_t* board);

#endif // ZOBRIST_H