#include "notation.h"
#include "extern/clib.h"
#include "piece.h"
#include <ctype.h>
#include <stdio.h>

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
        board->halfmove = atoi(halfmove);
        if(board->halfmove < 0) board->halfmove *= -1; // in case of negative number
    }

    if(!is_number(fullmove)){
        ERRO("full move clock should be a number");
    } else {
        board->fullmove = atoi(fullmove);
        if(board->fullmove < 0) board->fullmove *= -1; // in case of negative number
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
    char active_color = (board->turn == PIECE_COLOR_WHITE) ? 'w' : 'b';

    // 3. Castling Rights
    char castling_rights[5] = "";  // "KQkq"
    if (has_castling_rights(board, CASTLE_WHITE_KINGSIDE)) strcat(castling_rights, "K");
    if (has_castling_rights(board, CASTLE_WHITE_QUEENSIDE)) strcat(castling_rights, "Q");
    if (has_castling_rights(board, CASTLE_BLACK_KINGSIDE)) strcat(castling_rights, "k");
    if (has_castling_rights(board, CASTLE_BLACK_QUEENSIDE)) strcat(castling_rights, "q");
    if (strlen(castling_rights) == 0) {
        strcat(castling_rights, "-");  // No castling rights
    }

    // 4. En Passant Target Square
    char en_passant[3] = "-";  // Default to no en passant
    if (board->enpassant_square[0] != '-') {
        strcpy(en_passant, board->enpassant_square);
    }

    // 5. Halfmove Clock
    int halfmove_clock = board->halfmove;

    // 6. Fullmove Number
    int fullmove_number = board->fullmove;

    // Create the final FEN string
    sprintf(fen, "%s %c %s %s %d %d",
            board_fen,             // Piece placement
            active_color,          // Active color
            castling_rights,       // Castling rights
            en_passant,            // En passant
            halfmove_clock,        // Halfmove clock
            fullmove_number);      // Fullmove number
}

void pgn_export(game_t* game, char* pgn)
{
    sprintf(pgn, "[Event \"%s\"]\n", game->event);
    sprintf(pgn + strlen(pgn), "[Site \"%s\"]\n", game->site);
    sprintf(pgn + strlen(pgn), "[Date \"%s\"]\n", game->date);
    sprintf(pgn + strlen(pgn), "[White \"%s\"]\n", game->white);
    sprintf(pgn + strlen(pgn), "[Black \"%s\"]\n", game->black);
    sprintf(pgn + strlen(pgn), "[Result \"%s\"]\n", game->result);
    sprintf(pgn + strlen(pgn), "\n");

    for (int i = 0; i < game->move_count; i++) {
        if (i % 2 == 0) {
            sprintf(pgn + strlen(pgn), "%d. %s ", (i / 2) + 1, game->moves[i].move);
        } else {
            sprintf(pgn + strlen(pgn), "%s ", game->moves[i].move);
        }
    }
    sprintf(pgn + strlen(pgn), "\n");
}

void pgn_import(game_t* game, const char* pgn) {
    char line[1024];
    char* token;
    int move_count = 0;
    int reading_moves = 0;
    
    // Clear the game data
    memset(game, 0, sizeof(game_t));

    // Parse the PGN string
    const char* pgn_ptr = pgn;

    while (sscanf(pgn_ptr, "[Event \"%[^\"]\"]", game->event) ||
           sscanf(pgn_ptr, "[Site \"%[^\"]\"]", game->site) ||
           sscanf(pgn_ptr, "[Date \"%[^\"]\"]", game->date) ||
           sscanf(pgn_ptr, "[White \"%[^\"]\"]", game->white) ||
           sscanf(pgn_ptr, "[Black \"%[^\"]\"]", game->black) ||
           sscanf(pgn_ptr, "[Result \"%[^\"]\"]", game->result)) {

        // Advance the pointer past the current tag
        pgn_ptr = strchr(pgn_ptr, '\n') + 1;  // Skip to the next line
    }

    // Now extract the moves
    token = strtok((char*)pgn_ptr, "\n");
    while (token != NULL) {
        if (token[0] == '.') {
            // Skip move numbers (e.g., "1.", "2."), just record moves
            token = strtok(NULL, "\n");
            continue;
        }

        // Store the move
        strncpy(game->moves[move_count].move, token, sizeof(game->moves[move_count].move));
        move_count++;
        token = strtok(NULL, "\n");
    }
    game->move_count = move_count;
}
