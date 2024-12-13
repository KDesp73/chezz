#include "hashing.h"
#include "extern/clib.h"
#include "notation.h"
#include "zobrist.h"
#include "board.h"
#include "piece.h"
#include "square.h"
#include <stdio.h>
#include <stdlib.h>

void init_hash_table_(hash_table_t* table, size_t capacity, uint64_t starting_hash)
{
    if (!table) {
        ERRO("hash_table_t pointer is NULL.\n");
        exit(EXIT_FAILURE);
    }

    table->entries = calloc(capacity, sizeof(hash_entry_t));
    if (!table->entries) {
        ERRO("Memory allocation failed for hash_table entries.\n");
        exit(EXIT_FAILURE);
    }

    table->size = 0;
    table->capacity = capacity;

    // Adding starting position
    update_hash_table(table, starting_hash);
}

void init_hash_table(hash_table_t* table, size_t capacity, const char* starting_fen)
{
    if (!table) {
        ERRO("hash_table_t pointer is NULL.\n");
        exit(EXIT_FAILURE);
    }

    table->entries = calloc(capacity, sizeof(hash_entry_t));
    if (!table->entries) {
        ERRO("Memory allocation failed for hash_table entries.\n");
        exit(EXIT_FAILURE);
    }

    table->size = 0;
    table->capacity = capacity;

    // Adding starting position
    board_t board;
    fen_import(&board, (starting_fen) ? starting_fen : STARTING_FEN);
    update_hash_table(table, calculate_zobrist_hash(&board));
}

void free_hash_table(hash_table_t* table)
{
    free(table->entries);
}

_Bool update_hash_table(hash_table_t* table, uint64_t hash)
{
    for (size_t i = 0; i < table->size; i++) {
        if (table->entries[i].hash == hash) {
            table->entries[i].count++;
            return table->entries[i].count >= 3;
        }
    }

    if (table->size < table->capacity) {
        table->entries[table->size].hash = hash;
        table->entries[table->size].count = 1;
        table->size++;
    }

    return 0;
}

void init_zobrist()
{
    srand(12345);  // Fixed seed for reproducibility

    // Initialize zobrist_table
    for (int piece = 0; piece < PIECE_TYPES; piece++) {
        for (int rank = 0; rank < BOARD_SIZE; rank++) {
            for (int file = 0; file < BOARD_SIZE; file++) {
                uint64_t high = ((uint64_t)rand() & 0xFFFF) << 48;
                uint64_t mid = ((uint64_t)rand() & 0xFFFF) << 32;
                uint64_t low = ((uint64_t)rand() & 0xFFFFFFFF);
                zobrist_table[piece][rank][file] = high | mid | low;
            }
        }
    }

    // Initialize castling rights
    for (int i = 0; i < CASTLING_OPTIONS; i++) {
        zobrist_castling[i] = ((uint64_t)rand() << 32) | rand();
    }

    // Initialize en passant files
    for (int file = 0; file < BOARD_SIZE; file++) {
        zobrist_en_passant[file] = ((uint64_t)rand() << 32) | rand();
    }

    zobrist_black_to_move = ((uint64_t)rand() << 32) | rand();
}

int piece_to_index(char piece)
{
    switch (piece) {
        case 'P': return 0;  // White Pawn
        case 'N': return 1;  // White Knight
        case 'B': return 2;  // White Bishop
        case 'R': return 3;  // White Rook
        case 'Q': return 4;  // White Queen
        case 'K': return 5;  // White King
        case 'p': return 6;  // Black Pawn
        case 'n': return 7;  // Black Knight
        case 'b': return 8;  // Black Bishop
        case 'r': return 9;  // Black Rook
        case 'q': return 10; // Black Queen
        case 'k': return 11; // Black King
        default:  return -1; // Invalid piece
    }
}

uint64_t calculate_zobrist_hash(const board_t* board)
{
    uint64_t hash = 0;

    // Add piece positions
    for (int rank = 0; rank < BOARD_SIZE; rank++) {
        for (int file = 0; file < BOARD_SIZE; file++) {
            square_t square;
            square_from_coords(&square, rank, file);
            char piece = piece_at(board, square);

            if (piece != EMPTY_SQUARE) {
                int piece_index = piece_to_index(piece);
                if (piece_index >= 0 && piece_index < PIECE_TYPES) {
                    hash ^= zobrist_table[piece_index][rank][file];
                } else {
                    fprintf(stderr, "Invalid piece '%c' at rank %d, file %d\n", piece, rank, file);
                }
            }
        }
    }

    // Add castling rights
    if (has_castling_rights(board->state, CASTLE_WHITE_KINGSIDE)) hash ^= zobrist_castling[0];
    if (has_castling_rights(board->state, CASTLE_WHITE_QUEENSIDE)) hash ^= zobrist_castling[1];
    if (has_castling_rights(board->state, CASTLE_BLACK_KINGSIDE)) hash ^= zobrist_castling[2];
    if (has_castling_rights(board->state, CASTLE_BLACK_QUEENSIDE)) hash ^= zobrist_castling[3];

    // Add en passant target square
    if (board->enpassant_square[0] != '-') {
        square_t enpassant_square;
        if (square_from_name(&enpassant_square, board->enpassant_square)) {
            if (enpassant_square.x >= 0 && enpassant_square.x < BOARD_SIZE) {
                hash ^= zobrist_en_passant[enpassant_square.x];
            } else {
                fprintf(stderr, "Invalid en passant square: %s\n", board->enpassant_square);
            }
        } else {
            fprintf(stderr, "Failed to parse en passant square: %s\n", board->enpassant_square);
        }
    }

    if (board->state.turn == 0) {
        hash ^= zobrist_black_to_move;
    }

    return hash;
}
