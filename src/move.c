#include "move.h"
#include "board.h"
#define CLIB_IMPLEMENTATION
#include "extern/clib.h"
#include "piece.h"
#include "square.h"
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

void move_freely(board_t* board, square_t from, square_t to)
{
    char from_piece = board->grid[COORDS(from)];

    if(from_piece == ' ') return; // No piece to move
    
    board->grid[COORDS(from)] = ' ';
    board->grid[COORDS(to)] = from_piece;
}

_Bool move(board_t *board, square_t from, square_t to, char promotion)
{
    if (!move_is_valid(board, from, to)) {
        board->error = ERROR_INVALID_MOVE;
        return 0;
    }
    char from_before = piece_at(board, from);
    int color = piece_color(from_before);

    size_t piece_count_before = number_of_pieces(board, PIECE_COLOR_NONE); // Count all pieces before the move

    update_castling_rights(board, from);
    char* enpassant_square = update_enpassant_square(board, from, to);

    // Execute the move
    if(king_is_castling(board, from, to)){ 
        if(!king_can_castle(board, from, to)){
            board->error = ERROR_INVALID_MOVE;
            return 0;
        }
        king_castle(board, from, to);
    } else if(pawn_is_enpassanting(board, from, to)) {
        if(!pawn_can_enpassant(board, from, to)){
            board->error = ERROR_INVALID_MOVE;
            return 0;
        }
        pawn_enpassant(board, from, to);
    } else if(pawn_is_promoting(board, from, to)) {
        if(!pawn_promote(board, from, to, promotion)) {
            board->error = ERROR_INVALID_MOVE;
            return 0;
        }
    } else {
        move_freely(board, from, to);
    }
    strncpy(board->enpassant_square, enpassant_square, 3);

    size_t piece_count_after = number_of_pieces(board, PIECE_COLOR_NONE);

    if(board->turn == PIECE_COLOR_BLACK) board->fullmove++;

    update_halfmove(board, from, to, piece_count_before, piece_count_after, from_before);

    board->turn = !board->turn;

    // Check for the posibility of a result
    if(board->halfmove >= 50) board->result = RESULT_DRAW_DUE_TO_50_MOVE_RULE;
    if(is_checkmate(board)) board->result = (color == PIECE_COLOR_WHITE)
                                            ? RESULT_WHITE_WON
                                            : RESULT_BLACK_WON;
    if(is_stalemate(board)) board->result = RESULT_STALEMATE;
    if(is_insufficient_material(board)) board->result = RESULT_DRAW_DUE_TO_INSUFFICIENT_MATERIAL;
    if(is_threefold_repetition(board)) board->result = RESULT_DRAW_BY_REPETITION;

    return 1;
}

_Bool piece_can_move(board_t* board, square_t piece, square_t target)
{
    assert(board != NULL);

    switch (tolower(piece_at(board, piece))) {
        case 'k':
            return king_can_move(board, piece, target);
        case 'q':
            return queen_can_move(board, piece, target);
        case 'r':
            return rook_can_move(board, piece, target);
        case 'b':
            return bishop_can_move(board, piece, target);
        case 'n':
            return knight_can_move(board, piece, target);
        case 'p':
            return pawn_can_move(board, piece, target);
        default:
            return 0;
    }
}

_Bool move_is_valid(const board_t* board, square_t from, square_t to)
{
    if(!piece_can_move((board_t*) board, from, to)) {
        return 0;
    }

    int color = PIECE_COLOR(board, from);

    if(board->turn != color) {
        return 0;
    }

    if (piece_is_pinned((board_t*) board, from)) {
        board_t temp;
        memcpy(temp.grid, board->grid, 64);

        move_freely(&temp, from, to);
        return !IN_CHECK(&temp, color);
    }


    // If the king is in check make a move that resolves the check
    if(IN_CHECK((board_t*) board, color)){
        board_t temp;
        board_init_board(&temp, *board);

        move_freely(&temp, from, to);
        return !IN_CHECK(&temp, color);
    }

    return 1;
}

_Bool attack_is_valid(const board_t* board, square_t from, square_t to, _Bool strict)
{
    if(!piece_can_attack((board_t*) board, from, to, strict)) {
        return 0;
    }

    int color = PIECE_COLOR(board, from);

    if(strict && board->turn != color) {
        return 0;
    }

    if (piece_is_pinned((board_t*) board, from)) {
        board_t temp;
        memcpy(temp.grid, board->grid, 64);

        move_freely(&temp, from, to);
        return !IN_CHECK(&temp, color);
    }


    // If the king is in check make a move that resolves the check
    if(IN_CHECK((board_t*) board, color)){
        board_t temp;
        board_init_board(&temp, *board);

        move_freely(&temp, from, to);
        return !IN_CHECK(&temp, color);
    }

    return 1;
}

square_t** valid_moves(board_t* board, square_t piece, size_t* count)
{
    char _piece = piece_at(board, piece);
    int color = piece_color(_piece);

    size_t capacity = 10;
    *count = 0;
    square_t** moves = malloc(sizeof(square_t*) * capacity);
    if (!moves) return NULL;

    for (int rank = 0; rank < BOARD_SIZE; rank++) {
        for (int file = 0; file < BOARD_SIZE; file++) {
            square_t target;
            square_from_coords(&target, rank, file);
            char target_piece = piece_at(board, target);

            // Skip the piece's current square
            if (piece.rank == target.rank && piece.file == target.file) {
                continue;
            }

            // Skip king-related moves (if rule requires it)
            if (tolower(target_piece) == 'k') {
                continue;
            }

            // Handle pinned pieces
            if (piece_is_pinned(board, piece)) {
                board_t temp;
                memcpy(temp.grid, board->grid, 64);
                move_freely(&temp, piece, target);

                if (IN_CHECK(&temp, color)) {
                    continue;
                }
            }

            // Check move validity
            if (move_is_valid(board, piece, target)) {
                if (*count == capacity) {
                    void* temp = realloc(moves, sizeof(square_t*) * (capacity *= 2));
                    if (!temp) {
                        free(moves);
                        return NULL;
                    }
                    moves = temp;
                }
                moves[*count] = malloc(sizeof(square_t));
                if (!moves[*count]) {
                    for (size_t i = 0; i < *count; i++) free(moves[i]);
                    free(moves);
                    return NULL;
                }
                square_from_square(moves[*count], target);
                (*count)++;
            }
        }
    }

    // Optional: Shrink to fit
    void* temp = realloc(moves, sizeof(square_t*) * *count);
    if (temp) moves = temp;

    return moves;
}

_Bool move_name(board_t* board, const char* move_str)
{
    if (strlen(move_str) != 4 && strlen(move_str) != 5) {
        printf("Invalid move_str format. Use 4 or 5 characters (e.g., e2e4 or h7h8Q).\n");
        return 0;
    }

    char from[3], to[3], promotion;
    strncpy(from, move_str, 2);
    from[2] = '\0';
    strncpy(to, move_str + 2, 2);
    to[2] = '\0';
    promotion = move_str[4];

    square_t from_square, to_square;
    square_from_name(&from_square, from);
    square_from_name(&to_square, to);

    return move(board, from_square, to_square, promotion);
}

