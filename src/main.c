#include "ui.h"
#include "zobrist.h"

int main(int argc, char** argv){
    init_zobrist();

    tui_run(NULL);

    return 0;
}
