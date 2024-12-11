#include "notation.h"
#include "board.h"
#include "ui.h"
#include <assert.h>
#define CLIB_IMPLEMENTATION
#include "extern/clib.h"
#include "move.h"
#include "piece.h"
#include "square.h"
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include "common.h"

#undef DEBU
#define DEBU(fmt, ...)

_Bool is_number(const char* str) {
    if (str == NULL || *str == '\0') {
        return false; // Null or empty string is not a number
    }

    // Handle optional leading sign
    if (*str == '+' || *str == '-') {
        str++;
    }

    if (*str == '\0') {
        return false; // Only a sign without digits is not a number
    }

    // Check each character
    while (*str) {
        if (!isdigit((unsigned char)*str)) {
            return false; // Non-digit character found
        }
        str++;
    }

    return true; // All characters are digits
}


void fen_import(board_t *board, const char *fen)
{
    char b[71 + 1];  // Board layout (max length 71 characters)
    char turn;
    char castling[4 + 1];  // Castling rights (max 4 characters)
    char enpassant[2 + 1];  // En passant square (max 2 characters)
    char halfmove[3 + 1];   // Halfmove clock (max 3 characters)
    char fullmove[4 + 1];   // Fullmove counter (max 4 characters)

    // Parse the FEN string
    if (sscanf(fen, "%71s %c %4s %2s %3s %4s", 
               b, 
               &turn, 
               castling, 
               enpassant, 
               halfmove, 
               fullmove) != 6) {
        ERRO("Error reading input\n");
        return;
    }
    enpassant[2] = '\0';

    // Initialize the board state
    board->state.turn = (turn == 'w');  // White's turn if 'w', else Black's turn
    if(turn != 'w' && turn != 'b') {
        ERRO("Active color should be 'w' or 'b'. Defaulting to white.");
        board->state.turn = PIECE_COLOR_WHITE;
    }

    // Initialize castling rights to zero
    board->state.castling_rights = 0b0000;
    for(size_t i = 0; i < strlen(castling); i++) {
        switch (castling[i]) {
        case 'K': board->state.castling_rights |= CASTLE_WHITE_KINGSIDE; break;
        case 'Q': board->state.castling_rights |= CASTLE_WHITE_QUEENSIDE; break;
        case 'k': board->state.castling_rights |= CASTLE_BLACK_KINGSIDE; break;
        case 'q': board->state.castling_rights |= CASTLE_BLACK_QUEENSIDE; break;
        case '-': goto no_castling;
        default: WARN("Illegal character '%c' found in castling", castling[i]); break;
        }
    }
no_castling:

    // Copy the en passant square, ensure null-terminated string
    if(
        !strcmp(enpassant, "-") || 
        !square_is_valid(enpassant)
    ){
        // ERRO("Invalid enpassant target square '%s'", enpassant);
        strncpy(board->enpassant_square, "-", 3);
    } else {
        strncpy(board->enpassant_square, enpassant, 3);
    }

    if(!is_number(halfmove)){
        ERRO("half move clock should be a number");
    } else {
        board->state.halfmove = atoi(halfmove);
        if(board->state.halfmove < 0) board->state.halfmove *= -1; // in case of negative number
    }

    if(!is_number(fullmove)){
        ERRO("full move clock should be a number");
    } else {
        board->state.fullmove = atoi(fullmove);
        if(board->state.fullmove < 0) board->state.fullmove *= -1; // in case of negative number
    }

    square_t square;
    square_from_name(&square, "a8");
    size_t space = 0;
    size_t i = 0;

    while (i < strlen(b)) {
        if (b[i] == '/') {
            square.y--;  // Move to the next row
            square.x = 0;  // Reset file to 0 (a-file)
        } else if (isdigit(b[i])) {
            space = b[i] - '0';  // Set space to number value (skip this many squares)
            for (size_t j = 0; j < space; j++) {
                board->grid[COORDS(square)] = ' ';  // Fill empty squares with a placeholder
                square.x++;
            }
        } else {
            switch (tolower(b[i])) {
            case 'k':
            case 'q':
            case 'r':
            case 'b':
            case 'n':
            case 'p':
                board->grid[COORDS(square)] = b[i];
                square.x++;  // Move to the next square in the row
                break;
            default:
                ERRO("Illegal character in layout string: %c", b[i]);
            }
        }

        i++;  // Increment the index only once per loop iteration
    }
}

void fen_export(board_t* board, char fen[]) {
    memset(fen, 0, strlen(fen));

    // 1. Piece Placement
    int empty_count;
    int rank, file;
    char board_fen[256] = ""; // Buffer for the piece placement part of FEN

    for (rank = 7; rank >= 0; rank--) {
        empty_count = 0;
        for (file = 0; file < BOARD_SIZE; file++) {
            square_t square;
            square_from_coords(&square, rank, file);
            char piece = piece_at(board, square);
            if (piece == EMPTY_SQUARE) {
                empty_count++;
            } else {
                if (empty_count > 0) {
                    sprintf(board_fen + strlen(board_fen), "%d", empty_count);  // Add number of empty squares
                    empty_count = 0;
                }
                sprintf(board_fen + strlen(board_fen), "%c", piece);  // Add piece
            }
        }
        if (empty_count > 0) {
            sprintf(board_fen + strlen(board_fen), "%d", empty_count);  // Add number of empty squares at the end of the row
        }
        if (rank > 0) {
            sprintf(board_fen + strlen(board_fen), "/");  // Row separator
        }
    }

    // 2. Active Color
    char active_color = (board->state.turn == PIECE_COLOR_WHITE) ? 'w' : 'b';

    // 3. Castling Rights
    char castling_rights[5] = "";  // "KQkq"
    if (has_castling_rights(board->state, CASTLE_WHITE_KINGSIDE)) strcat(castling_rights, "K");
    if (has_castling_rights(board->state, CASTLE_WHITE_QUEENSIDE)) strcat(castling_rights, "Q");
    if (has_castling_rights(board->state, CASTLE_BLACK_KINGSIDE)) strcat(castling_rights, "k");
    if (has_castling_rights(board->state, CASTLE_BLACK_QUEENSIDE)) strcat(castling_rights, "q");
    if (strlen(castling_rights) == 0) {
        strcat(castling_rights, "-");  // No castling rights
    }

    // 4. En Passant Target Square
    char en_passant[3] = "-";  // Default to no en passant
    if (board->enpassant_square[0] != '-') {
        strcpy(en_passant, board->enpassant_square);
    }

    // 5. Halfmove Clock
    int halfmove_clock = board->state.halfmove;

    // 6. Fullmove Number
    int fullmove_number = board->state.fullmove;

    // Create the final FEN string
    sprintf(fen, "%s %c %s %s %d %d",
            board_fen,             // Piece placement
            active_color,          // Active color
            castling_rights,       // Castling rights
            en_passant,            // En passant
            halfmove_clock,        // Halfmove clock
            fullmove_number);      // Fullmove number
}

#define IS_EMPTY(x) (x[0] == '\0')

void pgn_export(game_t* game, char* pgn)
{
    sprintf(pgn, "[Event \"%s\"]\n", IS_EMPTY(game->event) ? "??" : game->event);
    sprintf(pgn + strlen(pgn), "[Site \"%s\"]\n", IS_EMPTY(game->site) ? "??" : game->site);
    sprintf(pgn + strlen(pgn), "[Date \"%s\"]\n", IS_EMPTY(game->date) ? "????.??.??" : game->date);

    if(!IS_EMPTY(game->fen) && !STREQ(game->fen, STARTING_FEN))
        sprintf(pgn + strlen(pgn), "[FEN \"%s\"]\n", game->fen);

    sprintf(pgn + strlen(pgn), "[White \"%s\"]\n", IS_EMPTY(game->white) ? "Player 1" : game->white);
    sprintf(pgn + strlen(pgn), "[Black \"%s\"]\n", IS_EMPTY(game->black) ? "Player 2" : game->black);

    sprintf(pgn + strlen(pgn), "[Result \"%s\"]\n", IS_EMPTY(game->result) ? "*" : game->result);

    sprintf(pgn + strlen(pgn), "\n");

    for (int i = 0; i < game->move_count; i++) {
        if (i % 2 == 0) {
            sprintf(pgn + strlen(pgn), "%d. %s ", (i / 2) + 1, game->moves[i].move);
        } else {
            sprintf(pgn + strlen(pgn), "%s ", game->moves[i].move);
        }
    }
    sprintf(pgn + strlen(pgn), "%s", IS_EMPTY(game->result) ? "*" : game->result);
    sprintf(pgn + strlen(pgn), "\n");
}

// TODO: has issues with moves
void pgn_import(game_t* game, const char* pgn)
{
    char line[1024];
    int move_count = 0;

    memset(game, 0, sizeof(game_t));

    const char* pgn_ptr = pgn;

    while (sscanf(pgn_ptr, "[Event \"%[^\"]\"]", game->event) ||
           sscanf(pgn_ptr, "[Site \"%[^\"]\"]", game->site) ||
           sscanf(pgn_ptr, "[Date \"%[^\"]\"]", game->date) ||
           sscanf(pgn_ptr, "[White \"%[^\"]\"]", game->white) ||
           sscanf(pgn_ptr, "[Black \"%[^\"]\"]", game->black) ||
           sscanf(pgn_ptr, "[Result \"%[^\"]\"]", game->result)) {

        pgn_ptr = strchr(pgn_ptr, '\n');
        if (pgn_ptr) pgn_ptr++;  // Skip to the next line
        else break;              // No more headers
    }

    const char* moves_start = pgn_ptr;
    while (*moves_start && *moves_start != '1') {
        moves_start++; // Skip non-move data
    }

    char* token;
    char moves_copy[4096];
    strncpy(moves_copy, moves_start, sizeof(moves_copy) - 1);
    moves_copy[sizeof(moves_copy) - 1] = '\0';

    token = strtok(moves_copy, " ");
    while (token != NULL) {
        // Skip move numbers (e.g., "1.", "2.")
        if (strchr(token, '.')) {
            token = strtok(NULL, " ");
            continue;
        }

        if (move_count < MAX_MOVES) {
            strncpy(game->moves[move_count].move, token, sizeof(game->moves[move_count].move) - 1);
            game->moves[move_count].move[sizeof(game->moves[move_count].move) - 1] = '\0';
            move_count++;
        }

        token = strtok(NULL, " ");
    }

    game->move_count = move_count;
}

void pgn_export_file(game_t* game, const char* path)
{
    char pgn[2048];
    pgn_export(game, pgn);

    clib_file_write(path, pgn, "w");
}

void remove_chars(char* str, const char* chars_to_remove)
{
    size_t i = 0, j = 0;
    size_t len = strlen(str);

    while (i < len) {
        // Check if the current character should be removed
        if (strchr(chars_to_remove, str[i]) == NULL) {
            str[j++] = str[i];
        }
        i++;
    }

    // Null-terminate the string
    str[j] = '\0';
}

void san_to_move(board_t *board, san_move_t san, square_t *from, square_t *to, char *promotion)
{
    // Initialize the output parameters
    *promotion = '\0';

    char* move = san.move;
    size_t len = strlen(move);


    if(STREQ(move, "O-O")){
        square_from_name(from, (board->state.turn) ? "e1" : "e8");
        square_from_name(to, (board->state.turn) ? "g1" : "g8");
        return;
    } else if(STREQ(move, "O-O-O")){
        square_from_name(from, (board->state.turn) ? "e1" : "e8");
        square_from_name(to, (board->state.turn) ? "c1" : "c8");
        return;
    }

    // Strip move of irrelevant information
    _Bool is_promotion = strchr(move, '=') != NULL;
    remove_chars(move, "x+#=");
    if(is_promotion){
        *promotion = move[strlen(move) - 1];
        move[strlen(move) - 1] = '\0';
    }
    DEBU("move: %s", move);

    char target[3];
    target[0] = move[strlen(move)-2];
    target[1] = move[strlen(move)-1];
    target[2] = '\0';
    DEBU("target square: %s", target);
    square_from_name(to, target);

    char piece = '\0';

    // Piece is a pawn
    if((strlen(move) == 3 && move[0] >= 'a' && move[0] <= 'h') || strlen(move) == 2){
        piece = (board->state.turn) ? 'P' : 'p';
    } else {
        piece = (board->state.turn) ? move[0] : tolower(move[0]);
    }

    size_t count;
    square_t** pieces = square_is_accessible_by(board, *to, piece, &count);

    if(count == 1) {
        square_from_name(from, pieces[0]->name);
        goto cleanup;
    }

    int file = 0;
    int rank = 0;

    // Move is: cd4
    if(strlen(move) == 3) file = move[0] - 'a' + 1;
    else if(strlen(move) == 4) { // Move is: Nbd2
        if(move[1] >= 'a' && move[1] <= 'h') file = move[1] - 'a' + 1;
        else rank = move[1] - '0';
    } else if(strlen(move) == 5) {
        char temp[3];
        temp[0] = move[1];
        temp[1] = move[2];
        temp[2] = '\0';
        square_from_name(from, temp);
        goto cleanup;
    } else return;

    if(file)
        DEBU("file %c", file + 'a' - 1);
    if(rank)
        DEBU("rank %d", rank);

    for(size_t i = 0; i < count; i++) {
        if(file && pieces[i]->file == file){
            square_from_name(from, pieces[i]->name);
            goto cleanup;
        }
        if(rank && pieces[i]->rank == rank) {
            square_from_name(from, pieces[i]->name);
            goto cleanup;
        }
    }

cleanup:
    squares_free(&pieces, count);
}

void move_to_san(board_t* board, square_t from, square_t to, char promotion, san_move_t* san)
{
    char piece = piece_at(board, from);
    int color = piece_color(piece);
    char* target_square = to.name;

    // Castling moves
    if (king_is_castling(board, from, to)) {
        if (to.file > from.file) {
            strcpy(san->move, "O-O"); // Kingside castling
        } else {
            strcpy(san->move, "O-O-O"); // Queenside castling
        }
        return;
    }

    // Default: normal move
    char piece_letter = (tolower(piece) == 'p') ? '\0' : toupper(piece); // Pawn moves omit the piece letter

    // Check if the move is a capture
    _Bool is_capture = board->grid[COORDS(to)] != ' ' || pawn_is_enpassanting(board, from, to);

    // Build SAN string
    if (piece_letter != '\0') {
        san->move[0] = piece_letter;
        san->move[1] = '\0'; // Ensure proper termination
    } else {
        san->move[0] = '\0';
    }
    DEBU("Initial san: %s", san->move);

    // Check if there is a need to specify file, rank, or both
    if (tolower(piece) == 'p') {
        // If the move is a pawn capture
        if (is_capture) {
            // Specify the file to disambiguate
            san->move[0] = 'a' + from.x;
            san->move[1] = 'x';
            san->move[2] = '\0'; // Ensure proper termination
        }
        DEBU("After disambiguation san: %s", san->move);
    } else {
        size_t ambiguities_count = 0;
        square_t** ambiguities = malloc(sizeof(square_t*) * 16); // Max number of ambiguities

        for (size_t rank = 0; rank < BOARD_SIZE; rank++) {
            for (size_t file = 0; file < BOARD_SIZE; file++) {
                square_t current;
                square_from_coords(&current, rank, file);

                // Skip empty squares
                if (board->grid[rank][file] == ' ') continue;

                // Skip our piece
                if (from.y == rank && from.x == file) continue;

                if (
                    piece == piece_at(board, current) &&
                    move_is_valid(board, current, to)
                ) {
                    ambiguities[ambiguities_count++] = square_new_coords(rank, file);
                }
            }
        }

        size_t ambiguity_count = 0;
        square_t* single_ambiguity = NULL;
        for (size_t i = 0; i < ambiguities_count; i++) {
            if (
                (from.x == ambiguities[i]->x || from.y == ambiguities[i]->y) ||
                (tolower(piece) == 'n')
            ) {
                single_ambiguity = ambiguities[i];
                ambiguity_count++;
            }
        }

        if (tolower(piece) == 'n' && !ambiguity_count && ambiguities_count)
            ambiguity_count = ambiguities_count;

        char disambiguation[3] = "";
        DEBU("ambiguity count: %zu", ambiguity_count);
        DEBU("ambiguities count: %zu", ambiguities_count);
        switch (ambiguity_count) {
        case 1:
            if (from.rank == single_ambiguity->rank) {
                sprintf(disambiguation, "%c", 'a' + (int)from.x);  // File disambiguation
            } else if (from.file == single_ambiguity->file) {
                sprintf(disambiguation, "%zu", from.rank);  // Rank disambiguation
            } else if (tolower(piece) == 'n') {
                sprintf(disambiguation, "%c", 'a' + (int)from.x);
            }
            break;
        case 0:
            break;
        default:
            sprintf(disambiguation, "%s", from.name);
            break;
        }
        strcat(san->move, disambiguation); // Proper termination ensured by strcat
        DEBU("After disambiguation san: %s", san->move);

        squares_free(&ambiguities, ambiguities_count);

        if (is_capture) {
            strcat(san->move, "x"); // Proper termination ensured by strcat
        }
    }
    DEBU("After takes san: %s", san->move);

    strcat(san->move, target_square); // Proper termination ensured by strcat
    DEBU("After target san: %s", san->move);

    // Promotion
    if (pawn_is_promoting(board, from, to)) {
        if (promotion) {
            char promo[3] = {'=', toupper(promotion), '\0'};
            strcat(san->move, promo); // Proper termination ensured by strcat
        }
    }
    DEBU("After promotion san: %s", san->move);

    board_t temp;
    board_init_board(&temp, *board);
    move(&temp, from, to, promotion);

    if (is_checkmate(&temp)) {
        strcat(san->move, "#"); // Proper termination ensured by strcat
    } else if (in_check(&temp, !color)) {
        strcat(san->move, "+"); // Proper termination ensured by strcat
    }
    board_free(&temp);
    DEBU("After mate or check san: %s", san->move);
}

void get_current_date(char* buffer, size_t buffer_size)
{
    time_t t = time(NULL);
    struct tm* tm_info = localtime(&t);

    // Format the date as YYYY.MM.DD
    strftime(buffer, buffer_size, "%Y.%m.%d", tm_info);
}

void game_init(game_t* game, 
    const char* event,
    const char* site,
    const char* white,
    const char* black,
    const char* fen
)
{
    game->move_count = 0;

    if(!event) game->event[0] = '\0';
    else game_set_event(game, event);

    if(!site) game->site[0] = '\0';
    else game_set_site(game, site);

    get_current_date(game->date, MAX_HEADER_LENGTH);

    if(!white) game->white[0] = '\0';
    else game_set_white(game, white);

    if(!black) game->black[0] = '\0';
    else game_set_black(game, black);

    if(!fen) game->fen[0] = '\0';
    else game_set_fen(game, fen);
}

void game_add_move(game_t* game, san_move_t move)
{
    if(game->move_count >= MAX_MOVES){
        ERRO("Maximum number of moves reached");
        return;
    }
    game->moves[game->move_count++] = move;
}

void game_set_event(game_t* game, const char* event)
{
    if(!event) return;
    strcpy(game->event, event);
}

void game_set_site(game_t* game, const char* site)
{
    if(!site) return;
    strcpy(game->site, site);
}
void game_set_date(game_t* game, const char* date)
{
    if(!date) return;
    strcpy(game->date, date);
}
void game_set_white(game_t* game, const char* white)
{
    if(!white) return;
    strcpy(game->white, white);
}
void game_set_black(game_t* game, const char* black)
{
    if(!black) return;
    strcpy(game->black, black);
}
void game_set_fen(game_t* game, const char* fen)
{
    if(!fen) return;
    strcpy(game->fen, fen);
}
void game_set_result(game_t* game, const char* result)
{
    if(!result) return;
    strcpy(game->result, result);
}

void game_print(game_t game)
{
    printf("[Event \"%s\"]\n", IS_EMPTY(game.event) ? "??" : game.event);
    printf("[Site \"%s\"]\n", IS_EMPTY(game.site) ? "??" : game.site);
    printf("[Date \"%s\"]\n", IS_EMPTY(game.date) ? "????.??.??" : game.date);

    if(!IS_EMPTY(game.fen) && !STREQ(game.fen, STARTING_FEN))
        printf("[FEN \"%s\"]\n", game.fen);

    printf("[White \"%s\"]\n", IS_EMPTY(game.white) ? "Player 1" : game.white);
    printf("[Black \"%s\"]\n", IS_EMPTY(game.black) ? "Player 2" : game.black);

    printf("[Result \"%s\"]\n", IS_EMPTY(game.result) ? "*" : game.result);

    printf("\n");

    for (int i = 0; i < game.move_count; i++) {
        if (i % 2 == 0) {
            printf("%d. %s ", (i / 2) + 1, game.moves[i].move);
        } else {
            printf("%s ", game.moves[i].move);
        }
    }
    printf("%s", IS_EMPTY(game.result) ? "*" : game.result);
    printf("\n");
}

void press_enter_to_continue()
{
    printf("Press Enter to continue...\n");
    getchar();  // Waits for the user to press Enter
}

void game_run(game_t game)
{
    board_t board;
    board_init_fen(&board, IS_EMPTY(game.fen) ? NULL : game.fen);
    clib_ansi_clear_screen();
    PRINT_FULL(&board, NULL);
    press_enter_to_continue();

    for(size_t i = 0; i < game.move_count; i++) {
        square_t from, to;
        char promotion;


        for(size_t j = 0; j < SCORE_COUNT; j++){
            if(STREQ(game.moves[i].move, result_score[j])) {
                board.state.result = i;
                goto end;
            }
        }

        san_to_move(&board, game.moves[i], &from, &to, &promotion);

        if(!move(&board, from, to, promotion)){
            // ERRO("Invalid pgn. Move %s is not valid", game.moves[i].move);
            goto end;
        }

        clib_ansi_clear_screen();
        PRINT_FULL(&board, &from, &to, NULL);
        press_enter_to_continue();
    }

end:
    if(board.state.result > 0) {
        printf("%s %s\n", result_message[board.state.result], result_score[board.state.result]);
    } else {
        printf("Game did not end\n");
    }
    board_free(&board);
}

void game_stream(game_t game, FILE* stream)
{
    assert(stream != NULL);

    board_t board;
    board_init_fen(&board, IS_EMPTY(game.fen) ? NULL : game.fen);

    // Stream initial state
    char fen[256];
    fen_export(&board, fen);
    fprintf(stream, "%s\n", fen);

    for (size_t i = 0; i < game.move_count; i++) {
        square_t from, to;
        char promotion;

        // Check for result condition
        for (size_t j = 0; j < SCORE_COUNT; j++) {
            if (STREQ(game.moves[i].move, result_score[j])) {
                board.state.result = j; // Store the result index
                goto end;
            }
        }

        // Parse and apply the move
        san_to_move(&board, game.moves[i], &from, &to, &promotion);
        if (!move(&board, from, to, promotion)) {
            // Invalid move detected
            fprintf(stream, "Invalid move: %s\n", game.moves[i].move);
            goto end;
        }

        // Export and stream the updated board state in FEN format
        fen_export(&board, fen);
        fprintf(stream, "%s\n", fen);
    }

end:
    // Stream game result or error
    if (board.state.result >= 0) {
        const char* message = result_message[board.state.result];
        const char* score = result_score[board.state.result];
        fprintf(stream, "Game Over: %s %s\n", message, score);
    } else {
        fprintf(stream, "Game did not end.\n");
    }

    board_free(&board);
}


