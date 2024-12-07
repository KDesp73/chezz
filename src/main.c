#include "ui.h"
#include "zobrist.h"

int main(int argc, char** argv){
    init_zobrist();

    ui_config_t config = FULL_CONFIG;
    config.perspective = 0;
    tui_run(NULL, config);

    return 0;
}
