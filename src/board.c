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

void board_print_highlight(const board_t* board, square_t** squares, size_t count)
{
    char* yellow_bg = (char*) COLOR_BG(214);
    const char* reset = ANSI_RESET;

    printf("┌───┬───┬───┬───┬───┬───┬───┬───┐\n");

    for (int i = 7; i >= 0; --i) {
        for (int j = 0; j <= 7; ++j) {
            int highlighted = 0;

            for (size_t sc = 0; sc < count; ++sc) {
                if (i == squares[sc]->rank - 1 && j == squares[sc]->file - 1) {
                    highlighted = 1;
                    break;
                }
            }

            printf("│");
            if (highlighted) {
                printf("%s %c %s", yellow_bg, board->grid[i][j], reset);
            } else {
                printf(" %c ", board->grid[i][j]);
            }
        }
        printf("│\n");

        if (i > 0) {
            printf("├───┼───┼───┼───┼───┼───┼───┼───┤\n");
        }
    }
    printf("└───┴───┴───┴───┴───┴───┴───┴───┘\n");

    free(yellow_bg);
}

void board_init(board_t* board)
{
    board_init_fen(board, STARTING_FEN);
}

void board_print(const board_t* board)
{
    board_print_highlight(board, NULL, 0);
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
    for(size_t i = 0; i < 4; i++) {
        switch (castling[i]) {
        case 'K': board->castling_rights |= WHITE_KINGSIDE; break;
        case 'Q': board->castling_rights |= WHITE_QUEENSIDE; break;
        case 'k': board->castling_rights |= BLACK_KINGSIDE; break;
        case 'q': board->castling_rights |= BLACK_QUEENSIDE; break;
        default: WARN("Illegal character '%c' found in castling", castling[i]); break;
        }
    }

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

int can_castle(const board_t* board, uint8_t castling_rights)
{
    return board->castling_rights & castling_rights;
}

void revoke_castling_rights(board_t* board, uint8_t castling_rights)
{
    board->castling_rights &= ~castling_rights;
}
