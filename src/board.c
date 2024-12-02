#include "board.h"
#include "piece.h"
#include "square.h"
#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define CLIB_IMPLEMENTATION
#include "extern/clib.h"

void board_print(const board_t* board, print_config_t config, square_t* first, ...)
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

    char* yellow_bg = (char*) COLOR_BG(214);
    const char* reset = ANSI_RESET;

    DEBU("error: %d", board->error);
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

    if(config.turn)
        printf("\n%s's turn\n", board->turn ? "White" : "Black");

    if(config.checks && board->checks != 0b00)
        printf("\n%s is in check!\n", white_in_check(board) ? "White" : "Black");

    free(yellow_bg);
}

void board_init(board_t* board)
{
    board_init_fen(board, STARTING_FEN);
    board->error = 0;
    board->checks = 0b00;
    board->checkmate = 0;
}

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

void board_init_fen(board_t* board, const char* fen)
{
    board->error = 0;
    board->checks = 0b00;

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

    // Initialize the board state
    board->turn = (turn == 'w');  // White's turn if 'w', else Black's turn
    if(turn != 'w' && turn != 'b') {
        ERRO("Active color should be 'w' or 'b'. Defaulting to white.");
        board->turn = PIECE_COLOR_WHITE;
    }

    // Initialize castling rights to zero
    board->castling_rights = 0b0000;
    for(size_t i = 0; i < strlen(castling); i++) {
        switch (castling[i]) {
        case 'K': board->castling_rights |= CASTLE_WHITE_KINGSIDE; break;
        case 'Q': board->castling_rights |= CASTLE_WHITE_QUEENSIDE; break;
        case 'k': board->castling_rights |= CASTLE_BLACK_KINGSIDE; break;
        case 'q': board->castling_rights |= CASTLE_BLACK_QUEENSIDE; break;
        case '-': goto no_castling;
        default: WARN("Illegal character '%c' found in castling", castling[i]); break;
        }
    }
no_castling:

    // Copy the en passant square, ensure null-terminated string
    if(!strcmp(enpassant, "- ") && (strlen(enpassant) != 2 || !square_from_name(enpassant))){
        ERRO("Invalid enpassant target square '%s'", enpassant);
        strncpy(board->enpassant_square, "- ", 3);
    } else {
        strncpy(board->enpassant_square, enpassant, 3);
    }

    if(!is_number(halfmove)){
        ERRO("half move clock should be a number");
    } else {
        board->halfmove = atoi(halfmove);
        if(board->halfmove < 0) board->halfmove *= -1; // in case of negative number
    }

    if(!is_number(fullmove)){
        ERRO("full move clock should be a number");
    } else {
        board->fullmove = atoi(fullmove);
        if(board->fullmove < 0) board->fullmove *= -1; // in case of negative number
    }

    square_t* square = square_from_name("a8");
    size_t space = 0;
    size_t i = 0;

    while (i < strlen(b)) {
        if (b[i] == '/') {
            square->y--;  // Move to the next row
            square->x = 0;  // Reset file to 0 (a-file)
        } else if (isdigit(b[i])) {
            space = b[i] - '0';  // Set space to number value (skip this many squares)
            for (size_t j = 0; j < space; j++) {
                board->grid[PCOORDS(square)] = ' ';  // Fill empty squares with a placeholder
                square->x++;
            }
        } else {
            switch (b[i]) {
            case 'K':
            case 'Q':
            case 'R':
            case 'B':
            case 'N':
            case 'P':
            case 'k':
            case 'q':
            case 'r':
            case 'b':
            case 'n':
            case 'p':
                board->grid[PCOORDS(square)] = b[i];
                square->x++;  // Move to the next square in the row
                break;
            default:
                ERRO("Illegal character in layout string: %c", b[i]);
            }
        }

        i++;  // Increment the index only once per loop iteration
    }
}


_Bool king_in_check(const board_t* board, int color)
{
    return (color == PIECE_COLOR_WHITE) ? white_in_check(board) : black_in_check(board);
}

_Bool white_in_check(const board_t* board)
{
    return board->checks & CHECK_WHITE_KING;
}
_Bool black_in_check(const board_t* board)
{
    return board->checks & CHECK_BLACK_KING;
}

int has_castling_rights(const board_t* board, uint8_t castling_rights)
{
    return board->castling_rights & castling_rights;
}

void revoke_castling_rights(board_t* board, uint8_t castling_rights)
{
    board->castling_rights &= ~castling_rights;
}

_Bool square_is_attacked(board_t *board, square_t* square, int color)
{
    assert(square != NULL);

    int x = square->x;
    int y = square->y;

    return square_is_attacked_coords(board, y, x, color);
}

_Bool square_is_attacked_fr(board_t *board, int rank, int file, int color)
{
    return square_is_attacked_coords(board, rank-1, file-1, color);
}

_Bool square_is_attacked_coords(board_t *board, int y, int x, int color)
{
    // Ensure the square coordinates are within bounds
    if (y < 0 || y >= BOARD_SIZE || x < 0 || x >= BOARD_SIZE) {
        return 0;
    }

    // Get the opposing color
    int enemy_color = (color == PIECE_COLOR_WHITE) ? PIECE_COLOR_BLACK : PIECE_COLOR_WHITE;

    // Check for pawn attacks
    char enemy_pawn = (enemy_color == PIECE_COLOR_WHITE) ? 'P' : 'p';
    if ((y + 1 < BOARD_SIZE && x + 1 < BOARD_SIZE && board->grid[y + 1][x + 1] == enemy_pawn) ||
        (y + 1 < BOARD_SIZE && x - 1 >= 0 && board->grid[y + 1][x - 1] == enemy_pawn) ||
        (y - 1 >= 0 && x + 1 < BOARD_SIZE && board->grid[y - 1][x + 1] == enemy_pawn) ||
        (y - 1 >= 0 && x - 1 >= 0 && board->grid[y - 1][x - 1] == enemy_pawn)) {
        return 1;
    }

    // Check for knight attacks
    char enemy_knight = (enemy_color == PIECE_COLOR_WHITE) ? 'N' : 'n';
    int knight_moves[8][2] = {
        {2, 1}, {2, -1}, {-2, 1}, {-2, -1},
        {1, 2}, {1, -2}, {-1, 2}, {-1, -2}
    };
    for (int i = 0; i < 8; i++) {
        int new_y = y + knight_moves[i][0];
        int new_x = x + knight_moves[i][1];
        if (new_y >= 0 && new_y < BOARD_SIZE && new_x >= 0 && new_x < BOARD_SIZE &&
            board->grid[new_y][new_x] == enemy_knight) {
            return 1;
        }
    }

    // Check for sliding piece attacks (rook, bishop, and queen)
    char enemy_rook = (enemy_color == PIECE_COLOR_WHITE) ? 'R' : 'r';
    char enemy_bishop = (enemy_color == PIECE_COLOR_WHITE) ? 'B' : 'b';
    char enemy_queen = (enemy_color == PIECE_COLOR_WHITE) ? 'Q' : 'q';

    // Rook/Queen: Horizontal and Vertical
    int directions[4][2] = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}};
    for (int i = 0; i < 4; i++) {
        for (int j = 1; j < BOARD_SIZE; j++) {
            int new_y = y + j * directions[i][0];
            int new_x = x + j * directions[i][1];
            if (new_y < 0 || new_y >= BOARD_SIZE || new_x < 0 || new_x >= BOARD_SIZE) break;
            char square = board->grid[new_y][new_x];
            if (square == EMPTY_SQUARE) continue;
            if (square == enemy_rook || square == enemy_queen) return 1;
            break;
        }
    }

    // Bishop/Queen: Diagonals
    int diagonals[4][2] = {{1, 1}, {1, -1}, {-1, 1}, {-1, -1}};
    for (int i = 0; i < 4; i++) {
        for (int j = 1; j < BOARD_SIZE; j++) {
            int new_y = y + j * diagonals[i][0];
            int new_x = x + j * diagonals[i][1];
            if (new_y < 0 || new_y >= BOARD_SIZE || new_x < 0 || new_x >= BOARD_SIZE) break;
            char square = board->grid[new_y][new_x];
            if (square == EMPTY_SQUARE) continue;
            if (square == enemy_bishop || square == enemy_queen) return 1;
            break;
        }
    }

    // Check for king attacks
    char enemy_king = (enemy_color == PIECE_COLOR_WHITE) ? 'K' : 'k';
    int king_moves[8][2] = {
        {1, 1}, {1, 0}, {1, -1},
        {0, 1},         {0, -1},
        {-1, 1}, {-1, 0}, {-1, -1}
    };
    for (int i = 0; i < 8; i++) {
        int new_y = y + king_moves[i][0];
        int new_x = x + king_moves[i][1];
        if (new_y >= 0 && new_y < BOARD_SIZE && new_x >= 0 && new_x < BOARD_SIZE &&
            board->grid[new_y][new_x] == enemy_king) {
            return 1;
        }
    }

    return 0; // Square is not under attack
}

square_t* find_king(board_t* board, int color)
{
    const char* home_name = (color == PIECE_COLOR_WHITE) ? "e1" : "e8";
    const char* queen_castling_name = (color == PIECE_COLOR_WHITE) ? "c1" : "c8";
    const char* king_castling_name = (color == PIECE_COLOR_WHITE) ? "g1" : "g8";
    char king_piece = (color == PIECE_COLOR_WHITE) ? 'K' : 'k';

    square_t* home = square_from_name(home_name);
    square_t* queen_castling = square_from_name(queen_castling_name);
    square_t* king_castling = square_from_name(king_castling_name);

    if (!home || !queen_castling || !king_castling) {
        square_free(&home);
        square_free(&queen_castling);
        square_free(&king_castling);
        return NULL;
    }

    // Searching high probability squares first
    if (board->grid[PCOORDS(home)] == king_piece) {
        square_free(&queen_castling);
        square_free(&king_castling);
        return home;
    }
    if (board->grid[PCOORDS(king_castling)] == king_piece){
        square_free(&home);
        square_free(&queen_castling);
        return king_castling;
    }
    if (board->grid[PCOORDS(queen_castling)] == king_piece) {
        square_free(&home);
        square_free(&king_castling);
        return queen_castling;
    }

    square_free(&home);
    square_free(&queen_castling);
    square_free(&king_castling);

    // Full board search
    int row_start = (color == PIECE_COLOR_WHITE) ? 0 : 7;
    int row_end = (color == PIECE_COLOR_WHITE) ? 8 : -1;
    int row_step = (color == PIECE_COLOR_WHITE) ? 1 : -1;

    for (int i = row_start; i != row_end; i += row_step) {
        for (int j = 0; j < 8; ++j) {
            if (board->grid[i][j] == king_piece) {
                return square_from_coords(i, j); // Ensure the caller frees this memory
            }
        }
    }

    return NULL;
}
