#include "move.h"
#include "board.h"
#include "extern/clib.h"
#include "piece.h"
#include "square.h"
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

void move(board_t* board, square_t* from, square_t* to)
{
    update_castling_rights(board, from);

    char from_piece = board->grid[PCOORDS(from)];

    if(from_piece == ' ') return; // No piece to move
    
    board->grid[PCOORDS(from)] = ' ';
    board->grid[PCOORDS(to)] = from_piece;
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

        move(&temp, (square_t*) from, (square_t*) to);
        return !IN_CHECK(&temp, color);
    }


    // If the king is in check make a move that resolves the check
    if(IN_CHECK((board_t*) board, color)){
        board_t temp;
        memcpy(temp.grid, board->grid, 64);

        move(&temp, (square_t*) from, (square_t*) to);
        return !IN_CHECK(&temp, color);
    }

    return piece_can_move((board_t*) board, from, to);
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
                move(&temp, (square_t*)piece, target);

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

_Bool king_is_castling(const board_t* board, const square_t* from, const square_t* to)
{
    char piece = piece_at((board_t*) board, from);
    int color = piece_color(piece);

    return strcmp(from->name, (color == PIECE_COLOR_WHITE) ? "e1" : "e8") == 0 &&
        abs((int)from->file - (int)to->file) == 2;
}

_Bool can_castle(board_t* board, square_t* from, square_t* to)
{
    // King is not in a starting square
    if (strcmp(from->name, "e1") && strcmp(from->name, "e8")) 
        return 0;

    char piece = piece_at(board, from);
    int color = piece_color(piece);
    int file_diff = (int) from->file - (int) to->file;

    if (color == PIECE_COLOR_WHITE) {
        return (file_diff == -2 && has_castling_rights(board, CASTLE_WHITE_KINGSIDE)) ||
               (file_diff == 2 && has_castling_rights(board, CASTLE_WHITE_QUEENSIDE));
    } else if (color == PIECE_COLOR_BLACK) {
        return (file_diff == -2 && has_castling_rights(board, CASTLE_BLACK_KINGSIDE)) ||
               (file_diff == 2 && has_castling_rights(board, CASTLE_BLACK_QUEENSIDE));
    }

    return 0;
}

void castle(board_t* board, square_t* from, square_t* to)
{
    update_castling_rights(board, from);

    char piece = piece_at(board, from);
    int color = piece_color(piece);
    int file_diff = (int) from->file - (int) to->file;

    square_t* kingside_rook = square_from_name(color == PIECE_COLOR_WHITE ? "h1" : "h8");
    square_t* queenside_rook = square_from_name(color == PIECE_COLOR_WHITE ? "a1" : "a8");
    square_t* kingside_rook_target = square_from_name(color == PIECE_COLOR_WHITE ? "f1" : "f8");
    square_t* queenside_rook_target = square_from_name(color == PIECE_COLOR_WHITE ? "d1" : "d8");

    if(file_diff == -2){
        move(board, from, to);
        move(board,kingside_rook, kingside_rook_target);
    } else if(file_diff == 2){
        move(board, from, to);
        move(board,queenside_rook, queenside_rook_target);
    }

    square_free(&kingside_rook);
    square_free(&queenside_rook);
    square_free(&kingside_rook_target);
    square_free(&queenside_rook_target);
}
