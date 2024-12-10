#ifndef NOTATION_H
#define NOTATION_H

#include "board.h"
#include <stdio.h>

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
    char fen[MAX_HEADER_LENGTH];
    san_move_t moves[MAX_MOVES];
    size_t move_count;
} game_t;

void game_init(game_t* game, 
    const char* event,
    const char* site,
    const char* white,
    const char* black,
    const char* fen
);

void game_stream(game_t game, FILE* stream);
void game_run(game_t game);
void game_print(game_t game);
void game_add_move(game_t* game, san_move_t move);
void game_set_event(game_t* game, const char* event);
void game_set_site(game_t* game, const char* site);
void game_set_date(game_t* game, const char* date);
void game_set_white(game_t* game, const char* white);
void game_set_black(game_t* game, const char* black);
void game_set_fen(game_t* game, const char* fen);
void game_set_result(game_t* game, const char* result);

void pgn_import(game_t* game, const char* pgn);
void pgn_export(game_t* game, char* pgn);
void pgn_export_file(game_t* game, const char* path);

void move_to_san(board_t* board, square_t from, square_t to, char promotion, san_move_t* san);
void san_to_move(board_t* board, san_move_t san, square_t* from, square_t* to, char* promotion);

#endif // NOTATION_H
