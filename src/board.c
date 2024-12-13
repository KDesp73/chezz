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

void board_init_board(board_t* board, board_t src)
{
    if (!board) return;

    // Copy the grid
    memcpy(board->grid, src.grid, sizeof(src.grid));

    // Copy turn
    board->state.turn = src.state.turn;

    // Copy en passant square
    strncpy(board->enpassant_square, src.enpassant_square, sizeof(board->enpassant_square));

    // Copy castling rights
    board->state.castling_rights = src.state.castling_rights;

    // Copy halfmove and fullmove counters
    board->state.halfmove = src.state.halfmove;
    board->state.fullmove = src.state.fullmove;

    // Copy error state
    board->state.error = src.state.error;

    // Copy result
    board->state.result = src.state.result;

    // Initialize the history hash table
    init_hash_table_(&board->state.history, src.state.history.capacity, calculate_zobrist_hash(board));

    // Copy history entries
    for (size_t i = 0; i < src.state.history.capacity; i++) {
        if (src.state.history.entries[i].hash) {
            board->state.history.entries[i] = src.state.history.entries[i];
            board->state.history.size++;
        }
    }
}

void board_init_fen(board_t* board, const char* fen)
{
    if(fen == NULL) {
        board_init_fen(board, STARTING_FEN);
        return;
    }

    fen_import(board, fen);

    board->state.error = 0;
    board->state.result = RESULT_NONE;
    init_hash_table(&board->state.history, 1000, fen);
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

square_t** square_is_accessible_by(const board_t* board, square_t square, char piece, size_t* count)
{
    square_t** squares = malloc(64 * sizeof(square_t*));
    if (!squares) return NULL;  // Check for malloc failure

    size_t square_count = 0;

    for(size_t rank = 0; rank < BOARD_SIZE; rank++){
        for(size_t file = 0; file < BOARD_SIZE; file++){
            square_t current;
            square_from_coords(&current, rank, file);
            
            if(board->grid[rank][file] == ' ') continue;
            if(rank == square.y && file == square.x) continue;
            if(piece != 0 && piece != 1){
                if(piece != piece_at(board, current)) continue;
            } else {
                if(piece != PIECE_COLOR(board, current)) continue;
            }
            

            if(move_is_valid((board_t*) board, current, square)){
                squares[square_count++] = square_new_coords(rank, file);
            }
        }
    }

    if (square_count == 0) {
        free(squares);
        *count = 0;
        return NULL;
    }

    squares = realloc(squares, square_count * sizeof(square_t*));

    *count = square_count;
    return squares;
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
        if (!attack_is_valid((board_t*) board, *current, square, 0)) {
            for (size_t j = i; attackers[j] != NULL; j++) {
                square_free(&attackers[j]);
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

uint8_t update_castling_rights(board_t* board, square_t from)
{
    char piece = piece_at(board, from);
    int color = piece_color(piece);

    // Moving a rook
    if(tolower(piece) == 'r'){
        if(color == PIECE_COLOR_WHITE && !strcmp(from.name, "h1")) return CASTLE_WHITE_KINGSIDE;
        else if(color == PIECE_COLOR_WHITE && !strcmp(from.name, "a1")) return CASTLE_WHITE_QUEENSIDE;
        else if(color == PIECE_COLOR_BLACK && !strcmp(from.name, "h8")) return CASTLE_BLACK_KINGSIDE;
        else if(color == PIECE_COLOR_BLACK && !strcmp(from.name, "a8")) return CASTLE_BLACK_QUEENSIDE;
    } else if(piece == 'k'){
        return 0b1100;
    } else if(piece == 'K'){
        return 0b0011;
    }

    return 0b0000;
}

void update_halfmove(board_t* board, square_t from, square_t to, size_t piece_count_before, size_t piece_count_after, char piece)
{
    int color = piece_color(piece);
    int direction = (color) ? 1 : -1;
    _Bool is_pawn_move = tolower(piece) == 'p';
    _Bool is_pawn_advancement = (tolower(piece) == 'p') && (from.rank == (color == PIECE_COLOR_WHITE) ? 7 : 2 && to.rank == from.rank + direction);
    _Bool is_capture = (piece_count_after != piece_count_before);

    if (is_pawn_advancement || is_capture || is_pawn_move) {
        board->state.halfmove = 0;
    } else {
        board->state.halfmove++;
    }
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

#undef DEBU
#define DEBU(fmt, ...)

square_t** attack_path_to_king(board_t* board, square_t king, square_t attacker, size_t* path_count)
{
    assert(board != NULL);
    assert(path_count != NULL);

    *path_count = 0; // Initialize the count of squares in the attack path
    square_t** path = NULL;

    char attacker_piece = tolower(piece_at(board, attacker));
    if (attacker_piece != 'q' && attacker_piece != 'r' && attacker_piece != 'b') {
        DEBU("Non-sliding pieces have no intermediate attack path");
        return path;
    }

    // Determine the direction of the attack
    int dx = (attacker.x > king.x) - (attacker.x < king.x); // +1, -1, or 0
    int dy = (attacker.y > king.y) - (attacker.y < king.y); // +1, -1, or 0
    dx = -dx;
    dy = -dy;

    // Ensure alignment for valid sliding piece paths
    if ((attacker_piece == 'r' && dx != 0 && dy != 0) ||  // Rooks can't move diagonally
        (attacker_piece == 'b' && (dx == 0 || dy == 0))) { // Bishops can't move straight
        DEBU("Invalid alignment");
        return path; 
    }

    // Calculate the attack path
    square_t current = attacker;
    while (true) {
        square_set_x(&current, current.x + dx);
        square_set_y(&current, current.y + dy);

        // Stop if out of bounds
        if (current.x < 0 || current.x >= BOARD_SIZE ||
            current.y < 0 || current.y >= BOARD_SIZE) {
            break; // Prevent infinite loop if something goes wrong
        }

        if (current.x == king.x && current.y == king.y) {
            DEBU("Reached the king, stop");
            break; 
        }

        // Allocate memory for the new square in the path
        path = realloc(path, (*path_count + 1) * sizeof(square_t*));
        if (!path) {
            perror("Memory allocation failed for attack path");
            exit(EXIT_FAILURE);
        }

        path[*path_count] = malloc(sizeof(square_t));
        if (!path[*path_count]) {
            perror("Memory allocation failed for attack path square");
            exit(EXIT_FAILURE);
        }

        square_from_coords(path[*path_count], current.y, current.x);
        (*path_count)++;
    }

    return path;
}

int has_castling_rights(state_t state, uint8_t castling_rights)
{
    return state.castling_rights & castling_rights;
}

void revoke_castling_rights(state_t* state, uint8_t castling_rights)
{
    state->castling_rights &= ~castling_rights;
}

