#include "board.h"
#include "move.h"
#include "notation.h"
#include "square.h"
#include "ui.h"
#define CLIB_IMPLEMENTATION
#include "extern/clib.h"
#include <stdio.h>
#include <inttypes.h>
#include "zobrist.h"
#include "piece.h"
#include "common.h"

void tui_run(const char* fen, ui_config_t config)
{
    game_t game;
    game_init(&game, NULL, "KDesp73 Chess", NULL, NULL, fen);

    board_t board;
    board_init_fen(&board, fen);
    clib_ansi_clear_screen();
    tui_board_print(&board, config, NULL);

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
            board.state.error = ERROR_INVALID_SQUARE;
            continue;
        }

        square_t from_square, to_square;
        square_from_name(&from_square, from);
        square_from_name(&to_square, to);

        char piece = piece_at(&board, from_square);
        if (piece == EMPTY_SQUARE) {
            clib_ansi_clear_screen();
            tui_board_print(&board, config, NULL);
            continue;
        }

        san_move_t san;
        move_to_san(&board, from_square, to_square, promotion, &san);

        if(!move(&board, from_square, to_square, promotion)){
            clib_ansi_clear_screen();
            tui_board_print(&board, config, NULL);
            continue;
        } else {
            board.state.error = 0;
        }

        game_add_move(&game, san);

        clib_ansi_clear_screen();
        tui_board_print(&board, config, &from_square, &to_square);

        if(board.state.result > 0){
            printf("%s %s\n", result_message[board.state.result], result_score[board.state.result]);
            board_free(&board);

            game_set_result(&game, result_score[board.state.result]);
            char pgn_string[2048];
            pgn_export(&game, pgn_string);

            printf("PGN:\n%s\n", pgn_string);
            return;
        }
    }
}

void tui_board_print_squares(const board_t* board, ui_config_t config, square_t** squares, size_t count)
{
    char* yellow_bg = (char*) COLOR_BG(214);
    const char* reset = ANSI_RESET;

    if(config.errors && board->state.error > 0){
        ERRO("%s", error_messages[board->state.error]);
    }

    const char* padding = "   ";
    printf("%s┌───┬───┬───┬───┬───┬───┬───┬───┐\n", (config.coords) ? padding : "");

    for (int i = (config.perspective) ? 7 : 0; (config.perspective) ? i >= 0 : i <= 7; i += (config.perspective) ? -1 : 1) {
        if(config.coords) 
            printf(" %d ", (!config.perspective ? 8 - i : i + 1));

        for (int j = 0; j <= 7; ++j) {
            int highlighted = 0;

            if(squares != NULL){
                for (size_t sc = 0; sc < count; ++sc) {
                    if(squares[sc] == NULL) continue;
                    if (i == squares[sc]->y && j == ((config.perspective) ? squares[sc]->x : 7 - squares[sc]->x)) {
                        highlighted = 1;
                        break;
                    }
                }
            }

            printf("│");
            char c = board->grid[i][(config.perspective) ? j : 7-j];
            if (config.highlights && highlighted) {
                printf("%s %c %s", yellow_bg, c, reset);
            } else {
                printf(" %c ", c);
            }
        }
        printf("│\n");

        if (i != (config.perspective ? 0 : 7)) {
            printf("%s├───┼───┼───┼───┼───┼───┼───┼───┤\n", (config.coords) ? padding : "");
        }
    }
    printf("%s└───┴───┴───┴───┴───┴───┴───┴───┘\n", (config.coords) ? padding : "");
    
    if(config.coords){
        printf("%s", padding);
        if(config.perspective) {
            for(int i = 0; i < BOARD_SIZE; i++){
                printf("  %c ", 'A' + i);
            }
        } else {
            for(int i = BOARD_SIZE - 1; i >= 0; i--){ // Reverse letters when perspective is 0
                printf("  %c ", 'A' + i);
            }
        }
        printf("\n");
    }

    if(config.castling){
        printf("\n");
        tui_print_castling_rights(board->state);
    }

    if(config.enpassant){
        printf("Enpassant square: %s\n", board->enpassant_square);
    }

    if(config.halfmove)
        printf("Halfmove: %zu\n", board->state.halfmove);
    if(config.fullmove)
        printf("Fullmove: %zu\n", board->state.fullmove);


    if(config.checks){
        if(in_check(board, PIECE_COLOR_WHITE))
            printf("White is in check!\n");
        if(in_check(board, PIECE_COLOR_BLACK))
            printf("Black is in check!\n");
    }

    if(config.turn)
        printf("%s's turn\n", board->state.turn ? "White" : "Black");

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

void tui_print_castling_rights(state_t state)
{
    const char* white_kingside = has_castling_rights(state, CASTLE_WHITE_KINGSIDE)
                                ? COLOR_BG(2)
                                : COLOR_BG(1);
    const char* white_queenside = has_castling_rights(state, CASTLE_WHITE_QUEENSIDE)
                                ? COLOR_BG(2)
                                : COLOR_BG(1);
    const char* black_kingside = has_castling_rights(state, CASTLE_BLACK_KINGSIDE)
                                ? COLOR_BG(2) 
                                : COLOR_BG(1);
    const char* black_queenside = has_castling_rights(state, CASTLE_BLACK_QUEENSIDE)
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

void TuiBoardPrintSquares(Board board, ui_config_t config, Square* squares, size_t count)
{
    const char* yellow_bg = "\033[48;5;214m"; // Background yellow color
    const char* reset = "\033[0m";             // Reset color formatting

    const char* padding = "   ";

    if (config.errors && board.state.error > 0) {
        fprintf(stderr, "Error: %s\n", error_messages[board.state.error]);
    }

    printf("%s┌───┬───┬───┬───┬───┬───┬───┬───┐\n", (config.coords) ? padding : "");

    for (int rank = (config.perspective ? 7 : 0);
         config.perspective ? rank >= 0 : rank <= 7;
         config.perspective ? rank-- : rank++) {

        if (config.coords) {
            printf(" %d ", config.perspective ? rank + 1 : 8 - rank);
        }

        for (int file = 0; file <= 7; ++file) {
            int highlighted = 0;

            if (squares != NULL) {
                for (size_t i = 0; i < count; ++i) {
                    int square_rank = squares[i] / 8;
                    int square_file = squares[i] % 8;

                    if (config.perspective) {
                        square_rank = 7 - square_rank;
                    }

                    if (rank == square_rank && file == square_file) {
                        highlighted = 1;
                        break;
                    }
                }
            }

            printf("│");

            char piece = ' '; // Default empty square
            for (int i = 0; i < PIECE_TYPE_COUNT; ++i) {
                if (board.bitboards[i] & (1ULL << (rank * 8 + file))) {
                    piece = "pnbrqkPNBRQK"[i];
                    break;
                }
            }

            if (config.highlights && highlighted) {
                printf("%s %c %s", yellow_bg, piece, reset);
            } else {
                printf(" %c ", piece);
            }
        }
        printf("│\n");

        if (rank != (config.perspective ? 0 : 7)) {
            printf("%s├───┼───┼───┼───┼───┼───┼───┼───┤\n", (config.coords) ? padding : "");
        }
    }
    printf("%s└───┴───┴───┴───┴───┴───┴───┴───┘\n", (config.coords) ? padding : "");

    if (config.coords) {
        printf("%s", padding);
        for (int file = 0; file < 8; ++file) {
            char label = config.perspective ? 'a' + file : 'h' - file;
            printf("  %c ", label);
        }
        printf("\n");
    }

    if (config.castling) {
        printf("\nCastling rights: ");
        if (board.state.castling_rights & 0b0001) printf("K");
        if (board.state.castling_rights & 0b0010) printf("Q");
        if (board.state.castling_rights & 0b0100) printf("k");
        if (board.state.castling_rights & 0b1000) printf("q");
        printf("\n");
    }

    if (config.enpassant && board.enpassant_square != 64) {
        char enpassant[3];
        SquareToName(enpassant, board.enpassant_square);
        printf("En passant square: %s\n", enpassant);
    }

    if (config.halfmove) {
        printf("Halfmove clock: %zu\n", board.state.halfmove);
    }

    if (config.fullmove) {
        printf("Fullmove number: %zu\n", board.state.fullmove);
    }

    if (config.checks) {
        // Check-related logic can be added here if needed
    }

    if (config.turn) {
        printf("%s's turn\n", board.state.turn ? "White" : "Black");
    }

    if (config.hash) {
        // Hash-related logic can be added here if needed
    }

    printf("\n");
}

void TuiBoardPrint(Board board, ui_config_t config, Square first, ...)
{
    Square* squares = NULL;
    int count = 0;

    if (first != 64) { // Use 64 as the terminator value
        va_list args;
        va_start(args, first);

        squares = (Square*)malloc(100 * sizeof(Square)); // Allocate memory for squares
        if (!squares) {
            perror("malloc failed");
            return;
        }

        squares[count++] = first;

        Square next_square;
        while ((next_square = va_arg(args, int)) != 64) { // Fetch arguments as int
            squares[count++] = next_square;
            if (count % 100 == 0) {
                squares = (Square*)realloc(squares, (count + 100) * sizeof(Square));
                if (!squares) {
                    perror("realloc failed");
                    va_end(args);
                    return;
                }
            }
        }
        va_end(args);

        // Resize to exact size
        squares = (Square*)realloc(squares, count * sizeof(Square));
        if (!squares) {
            perror("realloc failed");
            return;
        }
    }

    // Print the board with the collected squares
    TuiBoardPrintSquares(board, config, squares, count);

    // Free allocated memory
    if (squares) {
        free(squares);
        squares = NULL;
    }
}

