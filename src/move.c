#include "move.h"
#include "board.h"
#define CLIB_IMPLEMENTATION
#include "extern/clib.h"
#include "piece.h"
#include "square.h"
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

void move_freely(board_t* board, const square_t* from, const square_t* to)
{
    char from_piece = board->grid[PCOORDS(from)];

    if(from_piece == ' ') return; // No piece to move
    
    board->grid[PCOORDS(from)] = ' ';
    board->grid[PCOORDS(to)] = from_piece;
}

_Bool move(board_t *board, const square_t *from, const square_t *to)
{
    if (!move_is_valid(board, from, to)) {
        board->error = ERROR_INVALID_MOVE;
        return 0;
    }
    char from_before = piece_at(board, from);

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
    } else {
        move_freely(board, from, to);
    }
    strncpy(board->enpassant_square, enpassant_square, 3);

    size_t piece_count_after = number_of_pieces(board, PIECE_COLOR_NONE);

    if(board->turn == PIECE_COLOR_BLACK) board->fullmove++;

    update_halfmove(board, from, to, piece_count_before, piece_count_after, from_before);

    board->turn = !board->turn;

    update_checks(board);

    return 1;
}

_Bool piece_can_move(board_t* board, const square_t* piece, const square_t* target)
{
    assert(piece != NULL);
    assert(target != NULL);
    assert(board != NULL);

    switch (piece_at(board, piece)) {
        case 'K':
        case 'k':
            return king_can_move(board, piece, target);
        case 'Q':
        case 'q':
            return queen_can_move(board, piece, target);
        case 'R':
        case 'r':
            return rook_can_move(board, piece, target);
        case 'B':
        case 'b':
            return bishop_can_move(board, piece, target);
        case 'N':
        case 'n':
            return knight_can_move(board, piece, target);
        case 'P':
        case 'p':
            return pawn_can_move(board, piece, target);
        default:
            ERRO("Empty square");
            return 0;
    }
}

_Bool move_is_valid(const board_t* board, const square_t* from, const square_t* to)
{
    if(!piece_can_move((board_t*) board, from, to)) {
        return 0;
    }

    char piece = piece_at((board_t*)board, from);
    int color = piece_color(piece);

    if(board->turn != color) {
        return 0;
    }

    if (piece_is_pinned((board_t*) board, from)) {
        board_t temp;
        memcpy(temp.grid, board->grid, 64);

        move_freely(&temp, (square_t*) from, (square_t*) to);
        return !IN_CHECK(&temp, color);
    }


    // If the king is in check make a move that resolves the check
    if(IN_CHECK((board_t*) board, color)){
        board_t temp;
        memcpy(temp.grid, board->grid, 64);

        move_freely(&temp, (square_t*) from, (square_t*) to);
        return !IN_CHECK(&temp, color);
    }

    return 1;
}

square_t** valid_moves(board_t* board, const square_t* piece, size_t* count)
{
    char _piece = piece_at(board, piece);
    int color = piece_color(_piece);

    size_t capacity = 10;
    *count = 0;
    square_t** moves = malloc(sizeof(square_t*) * capacity);

    for (int rank = 0; rank < BOARD_SIZE; rank++) {
        for (int file = 0; file < BOARD_SIZE; file++) {
            square_t* target = square_from_coords(rank, file);
            char target_piece = piece_at(board, target);

            // Skip the piece's current square
            if (piece->rank == target->rank && piece->file == target->file) {
                square_free(&target);
                continue;
            }

            if (tolower(target_piece) == 'k') {
                square_free(&target);
                continue;
            }

            if (piece_is_pinned(board, piece)) {
                if(tolower(target_piece) == 'n') {
                    square_free(&target);
                    continue;
                }

                board_t temp = *board;
                move_freely(&temp, (square_t*)piece, target);

                if (IN_CHECK(&temp, color)) {
                    square_free(&target);
                    continue;
                }
            }

            if (piece_can_move(board, piece, target)) {
                if (*count == capacity) {
                    capacity *= 2;
                    moves = realloc(moves, sizeof(square_t*) * capacity);
                }
                moves[(*count)++] = target;
            } else {
                square_free(&target);
            }
        }
    }

    moves = realloc(moves, sizeof(square_t*) * *count);
    return moves;
}

