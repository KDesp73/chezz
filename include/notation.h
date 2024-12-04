#ifndef NOTATION_H
#define NOTATION_H

#include "board.h"

void fen_import(board_t* board, const char* fen);
void fen_export(board_t* board, char fen[]);

#define MAX_MOVES 300
#define MAX_HEADER_LENGTH 256

typedef struct {
    char move[16];  // For example, "e4", "Nf3"
} san_move_t;

typedef struct {
    char event[MAX_HEADER_LENGTH];
    char site[MAX_HEADER_LENGTH];
    char date[MAX_HEADER_LENGTH];
    char white[MAX_HEADER_LENGTH];
    char black[MAX_HEADER_LENGTH];
    char result[MAX_HEADER_LENGTH];
    san_move_t moves[MAX_MOVES];
    int move_count;
} game_t;

void pgn_import(game_t* game, const char* pgn);
void pgn_export(game_t* game, char* pgn);

void move_to_san(board_t* board, square_t from, square_t to, char promotion, san_move_t* san);

#endif // NOTATION_H
