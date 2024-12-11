#ifndef ZOBRIST_H
#define ZOBRIST_H

#include "board.h"
#include <stdint.h>

#define BOARD_SIZE 8
#define PIECE_TYPES 12
#define CASTLING_OPTIONS 4

static uint64_t zobrist_table[PIECE_TYPES][BOARD_SIZE][BOARD_SIZE];
static uint64_t zobrist_castling[CASTLING_OPTIONS];
static uint64_t zobrist_en_passant[BOARD_SIZE];
static uint64_t zobrist_black_to_move;

void init_zobrist();

uint64_t calculate_zobrist_hash(const board_t* board);
uint64_t CalculateZobristHash(const Board* board);

#endif // ZOBRIST_H
