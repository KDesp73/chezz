#include "ui.h"
#include "zobrist.h"
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char** argv){
    init_zobrist();

    tui_run(NULL);

    return 0;
}
