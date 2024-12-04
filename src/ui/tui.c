#include "board.h"
#include "move.h"
#include "notation.h"
#include "ui.h"
#define CLIB_IMPLEMENTATION
#include "extern/clib.h"
#include <stdio.h>
#include <inttypes.h>
#include "hashing.h"
#include "zobrist.h"
#include "piece.h"

void tui_run(const char* fen)
{
    board_t board;
    board_init_fen(&board, fen);
    clib_ansi_clear_screen();
    PRINT_FULL(&board, NULL);

    while (1) {
        char move_input[6]; // +1 for \0, +1 for safety
        printf("Enter move (e.g., e2e4): ");
        if (scanf("%5s", move_input) != 1) {
            printf("Invalid input. Try again.\n");
            continue;
        }

        if (strlen(move_input) != 4 && strlen(move_input) != 5) {
            printf("Invalid move format. Use 4 or 5 characters (e.g., e2e4 or h7h8Q).\n");
            continue;
        }

        char from[3], to[3], promotion;
        strncpy(from, move_input, 2);
        from[2] = '\0';
        strncpy(to, move_input + 2, 2);
        to[2] = '\0';
        promotion = move_input[4];

        if(!square_is_valid(from) || !square_is_valid(to)){
            printf("Invalid squares\n");
            continue;
        }

        square_t from_square, to_square;
        square_from_name(&from_square, from);
        square_from_name(&to_square, to);

        char piece = piece_at(&board, from_square);
        if (piece == EMPTY_SQUARE) {
            clib_ansi_clear_screen();
            PRINT_FULL(&board, NULL);
            continue;
        }

        san_move_t san;
        move_to_san(&board, from_square, to_square, promotion, &san);

        if(!move(&board, from_square, to_square, promotion)){
            clib_ansi_clear_screen();
            PRINT_FULL(&board, NULL);
            continue;
        }

        printf("%s\n", san.move);
        sleep(2);

        clib_ansi_clear_screen();
        PRINT_FULL(&board, &from_square, &to_square, NULL);

        if(board.result > 0){
            printf("%s %s\n", result_message[board.result], result_score[board.result]);
            board_free(&board);
            return;
        }
    }
}

void tui_board_print_squares(const board_t* board, ui_config_t config, square_t** squares, size_t count)
{
    char* yellow_bg = (char*) COLOR_BG(214);
    const char* reset = ANSI_RESET;

    if(config.errors && board->error > 0){
        ERRO("%s", error_messages[board->error]);
    }

    const char* padding = "   ";
    printf("%s┌───┬───┬───┬───┬───┬───┬───┬───┐\n", (config.coords) ? padding : "");

    for (int i = 7; i >= 0; --i) {
        if(config.coords) printf(" %d ", i + 1);
        for (int j = 0; j <= 7; ++j) {
            int highlighted = 0;

            if(squares != NULL){
                for (size_t sc = 0; sc < count; ++sc) {
                    if(squares[sc] == NULL) continue;
                    if (i == squares[sc]->rank - 1 && j == squares[sc]->file - 1) {
                        highlighted = 1;
                        break;
                    }
                }
            }

            printf("│");
            if (config.highlights && highlighted) {
                printf("%s %c %s", yellow_bg, board->grid[i][j], reset);
            } else {
                printf(" %c ", board->grid[i][j]);
            }
        }
        printf("│\n");

        if (i > 0) {
            printf("%s├───┼───┼───┼───┼───┼───┼───┼───┤\n", (config.coords) ? padding : "");
        }
    }
    printf("%s└───┴───┴───┴───┴───┴───┴───┴───┘\n", (config.coords) ? padding : "");
    
    if(config.coords){
        printf("%s", padding);
        for(int i = 0; i < BOARD_SIZE; i++){
            printf("  %c ", 'A' + i);
        }
        printf("\n");
    }

    if(config.castling){
        printf("\n");
        tui_print_castling_rights(board);
    }

    if(config.enpassant){
        printf("Enpassant square: %s\n", board->enpassant_square);
    }

    if(config.halfmove)
        printf("Halfmove: %zu\n", board->halfmove);
    if(config.fullmove)
        printf("Fullmove: %zu\n", board->fullmove);


    if(config.checks){
        if(in_check(board, PIECE_COLOR_WHITE))
            printf("White is in check!\n");
        if(in_check(board, PIECE_COLOR_BLACK))
            printf("Black is in check!\n");
    }

    if(config.turn)
        printf("%s's turn\n", board->turn ? "White" : "Black");

    if(config.hash)
        printf("Hash : 0x%" PRIx64 "\n", calculate_zobrist_hash(board));

    printf("\n");
    free(yellow_bg);
}

void tui_board_print(const board_t* board, ui_config_t config, square_t* first, ...)
{
    square_t** squares = NULL;
    int count = 0;
    if(first != NULL){
        va_list args;
        va_start(args, first);

        squares = (square_t**)malloc(100 * sizeof(square_t*));

        squares[count++] = first;

        square_t* next_square;
        while ((next_square = va_arg(args, square_t*)) != NULL) {
            squares[count++] = next_square;
        }
        va_end(args);

        squares = (square_t**)realloc(squares, (count + 1) * sizeof(square_t*));
    }

    tui_board_print_squares(board, config, squares, count);
    if(count > 0){
        free(squares);
        squares = NULL;
    }
}

void tui_print_castling_rights(const board_t* board)
{
    const char* white_kingside = has_castling_rights(board, CASTLE_WHITE_KINGSIDE)
                                ? COLOR_BG(2)
                                : COLOR_BG(1);
    const char* white_queenside = has_castling_rights(board, CASTLE_WHITE_QUEENSIDE)
                                ? COLOR_BG(2)
                                : COLOR_BG(1);
    const char* black_kingside = has_castling_rights(board, CASTLE_BLACK_KINGSIDE)
                                ? COLOR_BG(2) 
                                : COLOR_BG(1);
    const char* black_queenside = has_castling_rights(board, CASTLE_BLACK_QUEENSIDE)
                                ? COLOR_BG(2) 
                                : COLOR_BG(1);

    printf("Castling rights: %s K %s Q %s k %s q %s\n", 
        white_kingside,
        white_queenside,
        black_kingside,
        black_queenside,
        ANSI_RESET
        );

    freec(white_kingside);
    freec(white_queenside);
    freec(black_kingside);
    freec(black_queenside);
}

