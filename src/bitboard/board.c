#include "board.h"
#include <ctype.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "common.h"
#include "move.h"
#include "notation.h"
#include "piece.h"
#include "square.h"
#include "zobrist.h"

/**
 * Parses a FEN string and initializes the Board structure.
 */
void BoardInitFen(Board *board, const char *fen)
{
    if(fen == NULL) {
        BoardInitFen(board, STARTING_FEN);
        return;
    }

    FenImport(board, fen);

    board->state.error = 0;
    board->state.result = RESULT_NONE;
    init_hash_table(&board->state.history, 1000, fen);
}

/**
 * Prints a Board structure for debugging purposes.
 */
void BoardPrint(Board board)
{
    for (int rank = 7; rank >= 0; rank--) {
        for (int file = 0; file < 8; file++) {
            int square = rank * 8 + file;
            char piece = '.';
            for (int i = 0; i < PIECE_TYPE_COUNT; i++) {
                if (board.bitboards[i] & (1ULL << square)) {
                    piece = "pnbrqkPNBRQK"[i];
                    break;
                }
            }
            printf("%c ", piece);
        }
        printf("\n");
    }
    printf("\n");
    printf("Active color: %c\n", board.state.turn ? 'w' : 'b');
    printf("Castling rights: %x\n", board.state.castling_rights);
    printf("En passant square: %d\n", board.enpassant_square);
    printf("Halfmove clock: %zu\n", board.state.halfmove);
    printf("Fullmove number: %zu\n", board.state.fullmove);
}

// Function to check if a square is occupied by a piece of a given color
_Bool IsSquareOccupiedBy(const Board* board, Square square, uint8_t color)
{
    uint64_t occupied = 0;
    for (int i = color * 6; i < (color + 1) * 6; ++i) {
        occupied |= board->bitboards[i];
    }
    return (occupied & (1ULL << square)) != 0;
}

// Function to check if a square is empty
_Bool IsSquareEmpty(const Board* board, Square square)
{
    for (int i = 0; i < 12; ++i) {
        if (board->bitboards[i] & (1ULL << square)) {
            return 0;
        }
    }
    return 1;
}

_Bool IsSquareAttacked(Board board, Square square, uint8_t color)
{
    if (!IsSquareValid(square)) {
        return 0;
    }

    // Check for pawn attacks
    int direction = (color == PIECE_COLOR_WHITE) ? -1 : 1;
    Square left_attack = square + direction * BOARD_SIZE - 1;
    Square right_attack = square + direction * BOARD_SIZE + 1;

    if (IsSquareValid(left_attack) && board.bitboards[color * 6 + 0] & (1ULL << left_attack)) {
        return 1;
    }
    if (IsSquareValid(right_attack) && board.bitboards[color * 6 + 0] & (1ULL << right_attack)) {
        return 1;
    }

    // Check for knight attacks
    for (int i = 0; i < 8; ++i) {
        Square knight_square = square + KNIGHT_OFFSETS[i];
        if (IsSquareValid(knight_square) && (board.bitboards[color * 6 + 1] & (1ULL << knight_square))) {
            return 1;
        }
    }

    // Check for bishop and queen diagonal attacks
    for (int i = 0; i < 4; ++i) {
        Square current = square;
        while (1) {
            current += BISHOP_OFFSETS[i];
            if (!IsSquareValid(current)) {
                break;
            }
            if (!IsSquareEmpty(&board, current)) {
                if (board.bitboards[color * 6 + 2] & (1ULL << current) || // Bishop
                    board.bitboards[color * 6 + 4] & (1ULL << current)) { // Queen
                    return 1;
                }
                break;
            }
        }
    }

    // Check for rook and queen straight attacks
    for (int i = 0; i < 4; ++i) {
        Square current = square;
        while (1) {
            current += ROOK_OFFSETS[i];
            if (!IsSquareValid(current)) {
                break;
            }
            if (!IsSquareEmpty(&board, current)) {
                if (board.bitboards[color * 6 + 3] & (1ULL << current) || // Rook
                    board.bitboards[color * 6 + 4] & (1ULL << current)) { // Queen
                    return 1;
                }
                break;
            }
        }
    }

    // Check for king attacks
    for (int i = 0; i < 8; ++i) {
        Square king_square = square + KING_OFFSETS[i];
        if (IsSquareValid(king_square) && (board.bitboards[color * 6 + 5] & (1ULL << king_square))) {
            return 1;
        }
    }

    return 0;
}

_Bool IsKingInCheck(Board board, uint8_t color) {
    uint64_t king_bitboard = board.bitboards[color * 6 + 5];
    if (king_bitboard == 0) {
        fprintf(stderr, "No king found for color %d\n", color);
        return 0;
    }

    Square king_square = __builtin_ctzll(king_bitboard); // Get index of the first set bit
    return IsSquareAttacked(board, king_square, 1 - color);
}

/**
 * Caller is responsible for freeing the allocated memory
 */
Square UpdateEnpassantSquare(Board* board, Move move)
{
    Piece piece = PieceAt(board, GetFrom(move));

    if(tolower(piece.type) != 'p') {
        goto no_enpassant;
    }

    square_t from, to;
    square_from_fr(&from, Rank(GetFrom(move)), File(GetFrom(move)));

    int file_diff = abs((int)from.file - (int)to.file);
    int rank_diff = (int)to.rank - (int)from.rank;

    if(file_diff != 0) goto no_enpassant;

    if(rank_diff != ((piece.color) ? 2 : -2)) {
        goto no_enpassant;
    }

    return ((piece.color) ? 3 : 6) * 8 + to.file;

no_enpassant:
    return 64;
}

uint8_t UpdateCastlingRights(Board* board, Square from)
{
    Piece piece = PieceAt(board, from);
    int color = piece.color;
    char name[3];
    SquareToName(name, from);

    // Moving a rook
    if(tolower(piece.type) == 'r'){
        if(color == PIECE_COLOR_WHITE && !strcmp(name, "h1")) return CASTLE_WHITE_KINGSIDE;
        else if(color == PIECE_COLOR_WHITE && !strcmp(name, "a1")) return CASTLE_WHITE_QUEENSIDE;
        else if(color == PIECE_COLOR_BLACK && !strcmp(name, "h8")) return CASTLE_BLACK_KINGSIDE;
        else if(color == PIECE_COLOR_BLACK && !strcmp(name, "a8")) return CASTLE_BLACK_QUEENSIDE;
    } else if(piece.type == 'k'){
        return 0b1100;
    } else if(piece.type == 'K'){
        return 0b0011;
    }

    return 0b0000;
}

void UpdateHalfmove(Board* board, Move move, size_t piece_count_before, size_t piece_count_after, char piece)
{
    int color = piece_color(piece);
    int direction = (color) ? 1 : -1;
    _Bool is_pawn_move = tolower(piece) == 'p';
    _Bool is_pawn_advancement = (tolower(piece) == 'p') && (Rank(GetFrom(move)) == (color == PIECE_COLOR_WHITE) ? 7 : 2 && Rank(GetTo(move)) == Rank(GetFrom(move))+ direction);
    _Bool is_capture = (piece_count_after != piece_count_before);

    if (is_pawn_advancement || is_capture || is_pawn_move) {
        board->state.halfmove = 0;
    } else {
        board->state.halfmove++;
    }
}

size_t NumberOfPieces(const Board* board, uint8_t color)
{
    size_t count = 0;

    for (size_t i = 0; i < PIECE_TYPES; i++) {
        if ((color == PIECE_COLOR_BLACK && i < 6) || (color == PIECE_COLOR_WHITE && i >= 6)) {
            count += __builtin_popcountll(board->bitboards[i]);
        }
    }

    return count;
}

Square* SquareIsAttackedBy(const Board* board, Square square, int attacked_by, size_t* count)
{
    *count = 0;
    static Square attackedSquares[64];  // Array to hold attacked squares

    // Define bitboard for the attacking piece
    uint64_t attackedBitboard = 0;

    switch (attacked_by) {
        case PIECE_COLOR_WHITE:
        case PIECE_COLOR_BLACK:
            break;
        default:
            return NULL;
    }

    // Store attacked squares in attackedSquares array
    for (int i = 0; i < 64; i++) {
        if (attackedBitboard & (1ULL << i)) {
            attackedSquares[*count] = (Square)i;
            (*count)++;
        }
    }

    return attackedSquares;
}

Square* SquareIsAccessibleBy(const Board* board, Square square, char piece, size_t* count)
{
    *count = 0;
    static Square accessibleSquares[64];  // Array to hold accessible squares

    uint64_t accessibleBitboard = 0;

    switch (piece) {
        case 'R':
        case 'r':  // Rook
            accessibleBitboard = GenerateRookMoves(board, square);
            break;
        case 'B':
        case 'b':  // Bishop
            accessibleBitboard = GenerateBishopMoves(board, square);
            break;
        case 'N':
        case 'n':  // Knight
            accessibleBitboard = GenerateKnightMoves(board, square);
            break;
        case 'Q':
        case 'q':  // Queen
            accessibleBitboard = GenerateQueenMoves(board, square);
            break;
        case 'K':
        case 'k':  // King
            accessibleBitboard = GenerateKingMoves(board, square);
            break;
        case 'P':
            accessibleBitboard = GeneratePawnMoves(board, square, PIECE_COLOR_WHITE);
        case 'p':  // Pawn
            accessibleBitboard = GeneratePawnMoves(board, square, PIECE_COLOR_BLACK);
            break;
        default:
            return NULL;
    }

    // Store accessible squares in accessibleSquares array
    for (int i = 0; i < 64; i++) {
        if (accessibleBitboard & (1ULL << i)) {
            accessibleSquares[*count] = (Square)i;
            (*count)++;
        }
    }

    return accessibleSquares;
}

Square* AttackPathToKing(Board* board, Square king, Square attacker, size_t* path_count)
{
    *path_count = 0;
    static Square path[64];

    char attackerPiece = PieceAt(board, attacker).type;

    if (attackerPiece == 'r' || attackerPiece == 'q') {
        if (attacker % 8 == king % 8) {  // Same column
            int step = (king < attacker) ? -1 : 1;
            for (int i = attacker + step; i != king; i += step) {
                path[*path_count] = (Square)i;
                (*path_count)++;
            }
        } else if (attacker / 8 == king / 8) {  // Same row
            int step = (king < attacker) ? -1 : 1;
            for (int i = attacker + step; i != king; i += step) {
                path[*path_count] = (Square)i;
                (*path_count)++;
            }
        }
    } else if (attackerPiece == 'b' || attackerPiece == 'q') {
        int stepRow = (king / 8 > attacker / 8) ? 1 : -1;
        int stepCol = (king % 8 > attacker % 8) ? 1 : -1;
        int i = attacker + stepRow * 8 + stepCol;
        while (i != king) {
            path[*path_count] = (Square)i;
            (*path_count)++;
            i += stepRow * 8 + stepCol;
        }
    }

    return path;
}

Bitboard GetWhite(const Board* board)
{
    return board->bitboards[INDEX_WHITE_PAWN]
         | board->bitboards[INDEX_WHITE_KNIGHT]
         | board->bitboards[INDEX_WHITE_BISHOP]
         | board->bitboards[INDEX_WHITE_ROOK]
         | board->bitboards[INDEX_WHITE_QUEEN]
         | board->bitboards[INDEX_WHITE_KING];
}
Bitboard GetBlack(const Board* board)
{
    return board->bitboards[INDEX_BLACK_PAWN]
         | board->bitboards[INDEX_BLACK_KNIGHT]
         | board->bitboards[INDEX_BLACK_BISHOP]
         | board->bitboards[INDEX_BLACK_ROOK]
         | board->bitboards[INDEX_BLACK_QUEEN]
         | board->bitboards[INDEX_BLACK_KING];

}

