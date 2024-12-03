#include "board.h"
#include "hashing.h"
#include "move.h"
#include "notation.h"
#include "piece.h"
#include "square.h"
#include "zobrist.h"
#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define CLIB_IMPLEMENTATION
#include "extern/clib.h"
#include <inttypes.h>

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
        print_castling_rights(board);
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

    free(yellow_bg);
    printf("\n");
}

void board_init_board(board_t* board, board_t src)
{
    if (!board) return;

    // Copy the grid
    memcpy(board->grid, src.grid, sizeof(src.grid));

    // Copy turn
    board->turn = src.turn;

    // Copy en passant square
    strncpy(board->enpassant_square, src.enpassant_square, sizeof(board->enpassant_square));

    // Copy castling rights
    board->castling_rights = src.castling_rights;

    // Copy halfmove and fullmove counters
    board->halfmove = src.halfmove;
    board->fullmove = src.fullmove;

    // Copy error state
    board->error = src.error;

    // Copy result
    board->result = src.result;

    // Initialize the history hash table
    init_hash_table_(&board->history, src.history.capacity, calculate_zobrist_hash(board));

    // Copy history entries
    for (size_t i = 0; i < src.history.capacity; i++) {
        if (src.history.entries[i].hash) {
            board->history.entries[i] = src.history.entries[i];
            board->history.size++;
        }
    }
}

void board_free(board_t* board)
{
    free_hash_table(&board->history);
}

void board_init_fen(board_t* board, const char* fen)
{
    if(fen == NULL) {
        board_init_fen(board, STARTING_FEN);
        return;
    }

    board->error = 0;
    board->result = RESULT_NONE;
    init_hash_table(&board->history, 1000, fen);

    fen_import(board, fen);
}

int has_castling_rights(const board_t* board, uint8_t castling_rights)
{
    return board->castling_rights & castling_rights;
}

void revoke_castling_rights(board_t* board, uint8_t castling_rights)
{
    board->castling_rights &= ~castling_rights;
}

_Bool square_is_attacked(const board_t *board, square_t square, int attacked_by)
{
    return square_is_attacked_coords(board, square.y, square.x, attacked_by);
}

_Bool square_is_attacked_fr(const board_t *board, int rank, int file, int attacked_by)
{
    return square_is_attacked_coords(board, rank-1, file-1, attacked_by);
}

_Bool square_is_attacked_coords(const board_t *board, int y, int x, int attacked_by)
{
    // Ensure the square coordinates are within bounds
    if (y < 0 || y >= BOARD_SIZE || x < 0 || x >= BOARD_SIZE) {
        return 0;
    }

    // Determine piece symbols for the attacking color
    char enemy_pawn = (attacked_by == PIECE_COLOR_WHITE) ? 'P' : 'p';
    char enemy_knight = (attacked_by == PIECE_COLOR_WHITE) ? 'N' : 'n';
    char enemy_rook = (attacked_by == PIECE_COLOR_WHITE) ? 'R' : 'r';
    char enemy_bishop = (attacked_by == PIECE_COLOR_WHITE) ? 'B' : 'b';
    char enemy_queen = (attacked_by == PIECE_COLOR_WHITE) ? 'Q' : 'q';
    char enemy_king = (attacked_by == PIECE_COLOR_WHITE) ? 'K' : 'k';

    // Check for pawn attacks
    if (attacked_by == PIECE_COLOR_BLACK) {
        if ((y + 1 < BOARD_SIZE && x - 1 >= 0 && board->grid[y + 1][x - 1] == enemy_pawn) ||
            (y + 1 < BOARD_SIZE && x + 1 < BOARD_SIZE && board->grid[y + 1][x + 1] == enemy_pawn)) {
            return 1;
        }
    } else { // Attacked by white
        if ((y - 1 >= 0 && x - 1 >= 0 && board->grid[y - 1][x - 1] == enemy_pawn) ||
            (y - 1 >= 0 && x + 1 < BOARD_SIZE && board->grid[y - 1][x + 1] == enemy_pawn)) {
            return 1;
        }
    }

    // Check for knight attacks
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
            break; // Blocked by another piece
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
            break; // Blocked by another piece
        }
    }

    // Check for king attacks
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

square_t** square_is_attacked_by(const board_t* board, square_t square, int attacked_by, size_t* count)
{
    if (!board || square.rank < 1 || square.rank > BOARD_SIZE || 
        square.file < 1 || square.file > BOARD_SIZE) {
        return NULL;
    }

    // Set the enemy piece characters based on the color
    char enemy_pawn = (attacked_by == PIECE_COLOR_WHITE) ? 'P' : 'p';
    char enemy_knight = (attacked_by == PIECE_COLOR_WHITE) ? 'N' : 'n';
    char enemy_rook = (attacked_by == PIECE_COLOR_WHITE) ? 'R' : 'r';
    char enemy_bishop = (attacked_by == PIECE_COLOR_WHITE) ? 'B' : 'b';
    char enemy_queen = (attacked_by == PIECE_COLOR_WHITE) ? 'Q' : 'q';
    char enemy_king = (attacked_by == PIECE_COLOR_WHITE) ? 'K' : 'k';

    // Allocate memory for up to 64 attacking squares (maximum possible)
    square_t** attackers = malloc(64 * sizeof(square_t*));
    if (!attackers) return NULL;  // Check for malloc failure

    size_t attacker_count = 0;

    // Convert rank and file (1->8) to x and y (0->7)
    int y = square.rank - 1;
    int x = square.file - 1;

    // Check for pawn attacks
    if (attacked_by == PIECE_COLOR_BLACK) { // Black pawn attacks diagonally upwards
        if (y + 1 < BOARD_SIZE && x - 1 >= 0 && board->grid[y + 1][x - 1] == enemy_pawn) {
            attackers[attacker_count++] = square_new_coords(y + 1, x - 1);
        }
        if (y + 1 < BOARD_SIZE && x + 1 < BOARD_SIZE && board->grid[y + 1][x + 1] == enemy_pawn) {
            attackers[attacker_count++] = square_new_coords(y + 1, x + 1);
        }
    } else { // Attacked by white pawn, attacks diagonally downwards
        if (y - 1 >= 0 && x - 1 >= 0 && board->grid[y - 1][x - 1] == enemy_pawn) {
            attackers[attacker_count++] = square_new_coords(y - 1, x - 1);
        }
        if (y - 1 >= 0 && x + 1 < BOARD_SIZE && board->grid[y - 1][x + 1] == enemy_pawn) {
            attackers[attacker_count++] = square_new_coords(y - 1, x + 1);
        }
    }

    // Check for knight attacks
    int knight_moves[8][2] = {
        {2, 1}, {2, -1}, {-2, 1}, {-2, -1},
        {1, 2}, {1, -2}, {-1, 2}, {-1, -2}
    };
    for (int i = 0; i < 8; i++) {
        int new_y = y + knight_moves[i][0];
        int new_x = x + knight_moves[i][1];
        if (new_y >= 0 && new_y < BOARD_SIZE && new_x >= 0 && new_x < BOARD_SIZE &&
            board->grid[new_y][new_x] == enemy_knight) {
            attackers[attacker_count++] = square_new_coords(new_y, new_x);
        }
    }

    // Check for sliding piece attacks (rook, bishop, and queen)
    int directions[8][2] = {
        {0, 1}, {0, -1}, {1, 0}, {-1, 0}, // Rook/Queen directions
        {1, 1}, {1, -1}, {-1, 1}, {-1, -1} // Bishop/Queen directions
    };
    for (int i = 0; i < 8; i++) {
        for (int j = 1; j < BOARD_SIZE; j++) {
            int new_y = y + j * directions[i][0];
            int new_x = x + j * directions[i][1];
            if (new_y < 0 || new_y >= BOARD_SIZE || new_x < 0 || new_x >= BOARD_SIZE) break;
            char square = board->grid[new_y][new_x];
            if (square == EMPTY_SQUARE) continue;
            if ((i < 4 && (square == enemy_rook || square == enemy_queen)) || // Rook/Queen
                (i >= 4 && (square == enemy_bishop || square == enemy_queen))) { // Bishop/Queen
                attackers[attacker_count++] = square_new_coords(new_y, new_x);
            }
            break; // Blocked by another piece
        }
    }

    // Check for king attacks
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
            attackers[attacker_count++] = square_new_coords(new_y, new_x);
        }
    }

    // Terminate the array
    attackers[attacker_count] = NULL;
    *count = attacker_count;

    size_t i = 0;
    square_t* current = attackers[i];

    while (current != NULL) {
        if (!attack_is_valid(board, *current, square, 0)) {
            for (size_t j = i; attackers[j] != NULL; j++) {
                attackers[j] = attackers[j + 1];
            }
            *count -= 1;
        } else {
            i++;
        }
        current = attackers[i];
    }

    return attackers;
}

void find_king(square_t* square, const board_t* board, int color)
{
    const char* home_name = (color == PIECE_COLOR_WHITE) ? "e1" : "e8";
    const char* queen_castling_name = (color == PIECE_COLOR_WHITE) ? "c1" : "c8";
    const char* king_castling_name = (color == PIECE_COLOR_WHITE) ? "g1" : "g8";
    char king_piece = (color == PIECE_COLOR_WHITE) ? 'K' : 'k';

    square_t home, queen_castling, king_castling;
    square_from_name(&home, home_name);
    square_from_name(&queen_castling, queen_castling_name);
    square_from_name(&king_castling, king_castling_name);

    // Searching high probability squares first
    if (board->grid[COORDS(home)] == king_piece) {
        square_from_name(square, home_name);
        return;
    }
    if (board->grid[COORDS(king_castling)] == king_piece){
        square_from_name(square, king_castling_name);
        return;
    }
    if (board->grid[COORDS(queen_castling)] == king_piece) {
        square_from_name(square, queen_castling_name);
        return;
    }

    // Full board search
    int row_start = (color == PIECE_COLOR_WHITE) ? 0 : 7;
    int row_end = (color == PIECE_COLOR_WHITE) ? 8 : -1;
    int row_step = (color == PIECE_COLOR_WHITE) ? 1 : -1;

    for (int i = row_start; i != row_end; i += row_step) {
        for (int j = 0; j < 8; ++j) {
            if (board->grid[i][j] == king_piece) {
                square_from_coords(square, i, j);
                return;
            }
        }
    }
}

/**
 * Caller is responsible for freeing the allocated memory
 */
char* update_enpassant_square(board_t* board, square_t from, square_t to)
{
    char piece = piece_at(board, from);
    int color = piece_color(piece);

    if(tolower(piece) != 'p') {
        goto no_enpassant;
    }

    int file_diff = abs((int)from.file - (int)to.file);
    int rank_diff = (int)to.rank - (int)from.rank;

    if(file_diff != 0) goto no_enpassant;

    if(rank_diff != ((color) ? 2 : -2)) {
        goto no_enpassant;
    }

    square_t enpassant_square;
    square_from_coords(&enpassant_square, (color) ? 2 : 5, to.x);
    char* name = strdup(enpassant_square.name);
    return name;

no_enpassant:
    return strdup("-");
}

void update_castling_rights(board_t* board, square_t from)
{
    char piece = piece_at(board, from);
    int color = piece_color(piece);

    // Moving a rook
    if(tolower(piece) == 'r'){
        if(color == PIECE_COLOR_WHITE && !strcmp(from.name, "h1")) revoke_castling_rights(board, CASTLE_WHITE_KINGSIDE);
        else if(color == PIECE_COLOR_WHITE && !strcmp(from.name, "a1")) revoke_castling_rights(board, CASTLE_WHITE_QUEENSIDE);
        else if(color == PIECE_COLOR_BLACK && !strcmp(from.name, "h8")) revoke_castling_rights(board, CASTLE_BLACK_KINGSIDE);
        else if(color == PIECE_COLOR_BLACK && !strcmp(from.name, "a8")) revoke_castling_rights(board, CASTLE_BLACK_QUEENSIDE);
    } else if(piece == 'k'){
        revoke_castling_rights(board, 0b1100);
    } else if(piece == 'K'){
        revoke_castling_rights(board, 0b0011);
    }
}

void update_halfmove(board_t* board, square_t from, square_t to, size_t piece_count_before, size_t piece_count_after, char piece)
{
    int color = piece_color(piece);
    int direction = (color) ? 1 : -1;
    _Bool is_pawn_advancement = (tolower(piece) == 'p') && (from.rank == (color == PIECE_COLOR_WHITE) ? 7 : 2 && to.rank == from.rank + direction);
    _Bool is_capture = (piece_count_after != piece_count_before);

    if (is_pawn_advancement || is_capture) {
        board->halfmove = 0;
    } else {
        board->halfmove++;
    }
}

void print_castling_rights(const board_t* board)
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

size_t number_of_pieces(const board_t* board, int color)
{
    size_t count = 0;
    for(size_t i = 0; i < BOARD_SIZE; i++){
        for(size_t j = 0; j < BOARD_SIZE; j++){
            char piece = board->grid[j][i];
            switch(color){
            case PIECE_COLOR_WHITE:
                if(piece >= 'A' && piece <= 'Z') count++;
                break;
            case PIECE_COLOR_BLACK:
                if(piece >= 'a' && piece <= 'z') count++;
                break;
            case PIECE_COLOR_NONE:
                if(piece != ' ') count++;
                break;
            }
        }
    }
    return count;
}

_Bool in_check(const board_t *board, int color)
{
    square_t king;
    find_king(&king, board, color);
    return square_is_attacked(board, king, !color);
}
