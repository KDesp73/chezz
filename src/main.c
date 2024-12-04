#include "board.h"
#include "move.h"
#include "ui.h"
#include "notation.h"
#include "zobrist.h"
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char** argv){
    init_zobrist();

    // tui_run(NULL);

    const char* fen = NULL;

    game_t game;
    game_init(&game,
        "Test",
        "KDesp73 Chess",
        "Player 1",
        "Player 2",
        fen
        );


    board_t board;
    board_init_fen(&board, fen);

    MOVE(&board, &game, "f2f3");
    MOVE(&board, &game, "e7e5");
    MOVE(&board, &game, "g2g4");
    MOVE(&board, &game, "d8h4");

    PRINT_PLAIN(&board);

    char fen_str[1024];
    fen_export(&board, fen_str);
    printf("\n%s\n", fen_str);

    char pgn[2048];
    pgn_export(&game, pgn);
    printf("\n%s\n", pgn);


    return 0;
}
