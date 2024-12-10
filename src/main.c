#include "bitboard.h"
#include "version.h"
#include <stdlib.h>
#define CLIB_IMPLEMENTATION
#include "extern/clib.h"
#include "ui.h"
#include "zobrist.h"
#include <getopt.h>

int is_true(const char* str)
{
    return (
        STREQ(str, "1") ||
        STREQ(str, "true") ||
        STREQ(str, "TRUE") ||
        STREQ(str, "True") ||
        STREQ(str, "yes") ||
        STREQ(str, "y") ||
        STREQ(str, "YES") ||
        STREQ(str, "Yes")
    );
}

void main_exe(int argc, char** argv){
    CliArguments args = clib_cli_make_arguments(6,
        clib_cli_create_argument('h', "help", "Prints this message", no_argument),
        clib_cli_create_argument('v', "version", "Prints library version", no_argument),
        clib_cli_create_argument('F', "fen", "Specify the starting fen", required_argument),
        clib_cli_create_argument('i', "info", "Print additional information (boolean)", required_argument),
        clib_cli_create_argument('p', "perspective", "Print board from the perspective of {WHITE|BLACK}", required_argument),
        clib_cli_create_argument('e', "errors", "Print errors (boolean)", required_argument)
    );

    struct option* opts = clib_cli_get_options(args);
    char* fmt = clib_cli_generate_format_string(args);
    char* fen = NULL;

    ui_config_t config = FULL_CONFIG;
    _Bool full = 1;

    int opt;
    while ((opt = getopt_long(argc, argv, fmt, opts, NULL)) != -1) {
        switch (opt) {
        case 'h':
            clib_cli_help(args, "./chess [OPTIONS...]", "Written by KDesp73");
            goto cleanup;
        case 'v':
            printf("libchess v%s\n", CHESS_VERSION);
            goto cleanup;
        case 'F':
            fen = optarg;
            break;
        case 'i':
            full = (optarg == NULL) || is_true(optarg);
            break;
        case 'p':
            if(STREQ("BLACK", optarg)) config.perspective = 0;
            else config.perspective = 1;
            break;
        case 'e':
            config.errors = (optarg == NULL) | is_true(optarg);
            break;
        default:
            ERRO("Invalid option: '%c'", opt);
            goto cleanup;
        }
    }
    if(!full){
        config.hash = 0;
        config.turn = 0;
        config.checks = 0;
        config.castling = 0;
        config.fullmove = 0;
        config.halfmove = 0;
        config.enpassant = 0;
    }

    tui_run(fen, config);

cleanup:
    free(fmt);
    free(opts);
    clib_cli_clean_arguments(&args);

}

int main(int argc, char** argv){
    init_zobrist();

    Move move = MoveEncode(0, 63, PROMOTION_NONE, FLAG_NORMAL);
    Uint32Print(move);
    MovePrint(move);

    return 0;
}
