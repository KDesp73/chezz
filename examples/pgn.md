# PGN

## Export

```c
#include <chess/zobrist.h>
#include <chess/game.h>
#include <chess/board.h>
#include <chess/notation.h>

int main() {
    init_zobrist();
    
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

    char pgn[2048];
    pgn_export(&game, pgn);
    printf("%s\n", pgn);

    board_free(&board);
    return 0;
}
```

## Import

```c
#include <chess/zobrist.h>
#include <chess/game.h>
#include <chess/board.h>
#include <chess/notation.h>

int main() {
    init_zobrist();
    
    const char* pgn = read_file("game.pgn");
    game_t game;
    pgn_import(&game, pgn);

    // You can also run the game
    game_run(game);

    return 0;
}
```
