#include "ui.h"
#include "zobrist.h"
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char** argv){
    init_zobrist();

    tui_run("r1b2bnr/1p1pkp1p/p1p2qp1/4p3/1nB1P3/BP1P1N2/P1PN1PPP/R2Q1RK1 w - - 0 9");

    return 0;
}
