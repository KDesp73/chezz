#ifndef NOTATION_H
#define NOTATION_H

#include "board.h"

void fen_import(board_t* board, const char* fen);
void fen_export(board_t* board, char fen[]);

void pgn_import(board_t* board, const char* pgn);
void pgn_export(board_t* board, char* pgn);

#endif // NOTATION_H
