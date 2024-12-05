#include "board.h"
#include "move.h"
#include "ui.h"
#include "notation.h"
#include "zobrist.h"

int main(int argc, char** argv){
    init_zobrist();

    tui_run(NULL);

    return 0;
}
