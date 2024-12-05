# TUI

## 2 player game

```c
#include <chess/tui.h>
#include <chess/zobrist.h>

int main() {
    init_zobrist(); // Always run this once to init the hashing mechanism

    const char* fen = NULL; // Or any other valid fen
    tui_run(fen);
    
    return 0;
}
```

## Print Board

```c
#include <chess/tui.h>
#include <chess/board.h>

int main(){
    board_t board;
    board_init_fen(&board, "<fen>");

    // Setup the config as you want it
    ui_config_t config = {
        .errors = 0,
        .highlights = 1,
        .turn = 1,
        .checks,
        .coords = 1,
        .castling = 1,
        .enpassant = 0,
        .halfmove = 0,
        .fullmove = 0,
        .hash = 0,
    };

    tui_board_print(&board, config, NULL);
    // You can add squares to be highlighted if you like

    return 0;
}
```
