# FEN

## Export

```c
#include <chess/zobrist.h>
#include <chess/game.h>
#include <chess/board.h>
#include <chess/notation.h>

int main() {
    init_zobrist();
    
    const char* fen = NULL;

    board_t board;
    board_init_fen(&board, fen);

    // Make some moves
    MOVE(&board, &game, "f2f3");
    MOVE(&board, &game, "e7e5");
    MOVE(&board, &game, "g2g4");
    MOVE(&board, &game, "d8h4");

    char fen[256];
    fen_export(&game, fen);
    printf("%s\n", fen);

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
    
    // You shouldn't use the fen_import method since it only loads the board related info
    // Use board_init_fen instead for proper initialization

    const char* fen = "8/8/5R2/1k6/8/1r6/5R2/2K5 w - - 0 1";
    board_t board;
    fen_import(&board, fen);

    board_free(&board);

    return 0;
}
```


