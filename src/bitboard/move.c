#include "board.h"
#include "move.h"
#include "extern/clib.h"
#include "piece.h"
#include "square.h"
#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

Move MoveEncodeNames(const char* from, const char* to, uint8_t promotion, uint8_t flag)
{
    return MoveEncode(NameToSquare(from), NameToSquare(to), promotion, flag);
}

Move MoveEncode(Square from, Square to, uint8_t promotion, uint8_t flag)
{
    return (from & 0x3F) | 
           ((to & 0x3F) << 6) | 
           ((promotion & 0xF) << 12) | 
           ((flag & 0x7) << 16);
}

void MoveDecode(Move move, Square* from, Square* to, uint8_t* promotion, uint8_t* flag)
{
    *from = move & 0x3F;
    *to = (move >> 6) & 0x3F;
    *promotion = (move >> 12) & 0xF;
    *flag = (move >> 16) & 0x7;
}

Square GetFrom(Move move)
{
    Square from, to;
    uint8_t promotion, flags;
    MoveDecode(move, &from, &to, &promotion, &flags);

    return from;
}
Square GetTo(Move move)
{
    Square from, to;
    uint8_t promotion, flags;
    MoveDecode(move, &from, &to, &promotion, &flags);

    return to;
}
uint8_t GetPromotion(Move move)
{
    Square from, to;
    uint8_t promotion, flags;
    MoveDecode(move, &from, &to, &promotion, &flags);

    return promotion;
}
uint8_t GetFlag(Move move)
{
    Square from, to;
    uint8_t promotion, flags;
    MoveDecode(move, &from, &to, &promotion, &flags);

    return flags;
}

void MoveFreely(Board* board, Move move, uint8_t color)
{
    Square from, to;
    uint8_t promotion, flags;
    MoveDecode(move, &from, &to, &promotion, &flags);

    uint64_t from_bb = 1ULL << from;
    uint64_t to_bb = 1ULL << to;

    // Find the piece that moved
    for (int piece = 0; piece < 6; piece++) {
        if (board->bitboards[color * 6 + piece] & from_bb) {
            // Move the piece
            board->bitboards[color * 6 + piece] ^= from_bb; // Remove from source
            if (promotion) {
                // Add promoted piece
                board->bitboards[color * 6 + promotion - 1] |= to_bb;
            } else {
                // Move to destination
                board->bitboards[color * 6 + piece] |= to_bb;
            }
            break;
        }
    }

    // Handle captures
    int opponent = 1 - color;
    for (int piece = 0; piece < 6; piece++) {
        if (board->bitboards[opponent * 6 + piece] & to_bb) {
            board->bitboards[opponent * 6 + piece] ^= to_bb;
            break;
        }
    }

    // Special moves
    if (flags == 1) {
        // Handle castling
        if (to == 6) { // Kingside castling
            board->bitboards[color * 6 + 0] ^= (1ULL << 7) | (1ULL << 5); // Move rook
        } else if (to == 2) { // Queenside castling
            board->bitboards[color * 6 + 0] ^= (1ULL << 0) | (1ULL << 3); // Move rook
        }
    } else if (flags == 2) {
        // Handle en passant
        board->bitboards[opponent * 6 + 5] ^= (1ULL << (to - (color ? 8 : -8)));
    }
}

void MovePrint(Move move)
{
    Square from, to;
    uint8_t promotion, flags;
    MoveDecode(move, &from, &to, &promotion, &flags);

    char square_from[3], square_to[3];
    SquareToName(square_from, from);
    SquareToName(square_to, to);
    printf("Move: %s -> %s, Promotion: %d, Flags: %d\n",
            square_from, square_to,
            promotion, flags);
}

_Bool MoveIsValid(const Board* board, Move move, uint8_t color)
{
    Square from, to;
    uint8_t promotion, flags;
    MoveDecode(move, &from, &to, &promotion, &flags);
    uint64_t from_bb = 1ULL << from;
    uint64_t to_bb = 1ULL << to;

    for (int piece = 0; piece < 6; piece++) {
        if (board->bitboards[color * 6 + piece] & from_bb) {
            return 1;
        }
    }

    return 0;
}

char PromotionToChar(uint8_t promotion)
{
    switch (promotion) {
    case PROMOTION_QUEEN: return 'Q';
    case PROMOTION_ROOK: return 'R';
    case PROMOTION_BISHOP: return 'B';
    case PROMOTION_KNIGHT: return 'N';
    case PROMOTION_NONE: 
    default:
          return '\0';
    }
}

uint8_t CharToPromotion(char promotion)
{
    switch (promotion) {
    case 'Q': return PROMOTION_QUEEN;
    case 'R': return PROMOTION_ROOK;
    case 'B': return PROMOTION_BISHOP;
    case 'N': return PROMOTION_KNIGHT;
    default:
          return PROMOTION_NONE;
    }
}

_Bool PieceCanMove(const Board* board, Square from, Square to, Flag* flag)
{
    Piece piece = PieceAt(board, from);

    if (piece.type == ' ') {
        return 0;
    }

    *flag = FLAG_NORMAL;
    switch (tolower(piece.type)) {
        case 'p':
            return CanMovePawn(board, from, to, piece.color, flag);
        case 'n':
            return CanMoveKnight(board, from, to, piece.color);
        case 'b':
            return CanMoveBishop(board, from, to, piece.color);
        case 'r':
            return CanMoveRook(board, from, to, piece.color);
        case 'q':
            return CanMoveQueen(board, from, to, piece.color);
        case 'k':
            return CanMoveKing(board, from, to, piece.color, flag);
        default:
            return 0; // Unsupported piece type
    }
}

// _Bool MoveMake(Board* board, Move move)
// {
//     Square from, to;
//     uint8_t promotion, flag;
//     MoveDecode(move, &from, &to, &promotion, &flag);
//
//     Piece from_before = PieceAt(board, from);
//     int color = from_before.color;
//
//     size_t piece_count_before = NumberOfPieces(board, PIECE_COLOR_NONE); // Count all pieces before the move
//
//     uint8_t castling_rights_to_revoke = UpdateCastlingRights(board, from);
//     Square enpassant_square = UpdateEnpassantSquare(board, move);
//
//     // Execute the move
//     if(king_is_castling(board, from, to)){ 
//         if(!king_can_castle(board, from, to)){
//             board->state.error = ERROR_INVALID_MOVE;
//             return 0;
//         }
//         king_castle(board, from, to);
//     } else if(pawn_is_enpassanting(board, from, to)) {
//         if(!pawn_can_enpassant(board, from, to)){
//             board->state.error = ERROR_INVALID_MOVE;
//             return 0;
//         }
//         pawn_enpassant(board, from, to);
//     } else if(pawn_is_promoting(board, from, to)) {
//         if(!pawn_promote(board, from, to, promotion)) {
//             board->state.error = ERROR_INVALID_MOVE;
//             return 0;
//         }
//     } else {
//         MoveFreely(board, from, to);
//     }
//
//     revoke_castling_rights(&board->state, castling_rights_to_revoke);
//     board->enpassant_square = enpassant_square;
//
//     size_t piece_count_after = NumberOfPieces(board, PIECE_COLOR_NONE);
//
//     if(board->state.turn == PIECE_COLOR_BLACK) board->state.fullmove++;
//
//     UpdateHalfmove(board, move, piece_count_before, piece_count_after, from_before.type);
//
//     board->state.turn = !board->state.turn;
//
//     // Check for the posibility of a result
//     if(board->state.halfmove >= 50) board->state.result = RESULT_DRAW_DUE_TO_50_MOVE_RULE;
//     if(IsCheckmate(board)) board->state.result = (color == PIECE_COLOR_WHITE)
//                                             ? RESULT_WHITE_WON
//                                             : RESULT_BLACK_WON;
//     if(IsStalemate(board)) board->state.result = RESULT_STALEMATE;
//     if(IsInsufficientMaterial(board)) board->state.result = RESULT_DRAW_DUE_TO_INSUFFICIENT_MATERIAL;
//     if(IsThreefoldRepetition(board)) board->state.result = RESULT_DRAW_BY_REPETITION;
//
//     return 1;
// }

Bitboard GeneratePieceMoves(const Board* board, Square from, const int offsets[], size_t count, _Bool multiple_steps)
{
    Bitboard moves = 0;

    // Loop through each possible offset (direction)
    for (size_t i = 0; i < count; i++) {
        Square to = from;
        size_t step = 0;
        
        while (1) {
            to += offsets[i];
            step++;

            if (to < 0 || to >= 64) break;

            if (abs((from % 8) - (to % 8)) > 7 || abs((from / 8) - (to / 8)) > 7) break;

            moves |= (1ULL << to);

            if (!multiple_steps) break;
        }
    }

    return moves;
}

Bitboard GenerateKnightMoves(const Board* board, Square from)
{
    return GeneratePieceMoves(board, from, KNIGHT_OFFSETS, KNIGHT_OFFSETS_COUNT, 0);
}

Bitboard GenerateRookMoves(const Board* board, Square from)
{
    return GeneratePieceMoves(board, from, ROOK_OFFSETS, ROOK_OFFSETS_COUNT, 1);
}
Bitboard GenerateBishopMoves(const Board* board, Square from)
{
    return GeneratePieceMoves(board, from, BISHOP_OFFSETS, BISHOP_OFFSETS_COUNT, 1);
}
Bitboard GenerateQueenMoves(const Board* board, Square from)
{
    return GeneratePieceMoves(board, from, ROOK_OFFSETS, ROOK_OFFSETS_COUNT, 1)
         | GeneratePieceMoves(board, from, BISHOP_OFFSETS, BISHOP_OFFSETS_COUNT, 1);
}
Bitboard GenerateKingMoves(const Board* board, Square from)
{
    return GeneratePieceMoves(board, from, KING_OFFSETS, KING_OFFSETS_COUNT, 0);
}
Bitboard GeneratePawnMoves(const Board* board, Square from, uint8_t color)
{
    Bitboard moves = 0;
    int direction = (color == PIECE_COLOR_WHITE) ? 1 : -1;

    // Regular move (1 step forward)
    Square to = from + direction * 8;
    if (to >= 0 && to < 64) {
        if ((board->bitboards[INDEX_BLACK_PAWN] | board->bitboards[INDEX_WHITE_PAWN]) & (1ULL << to)) {
            // The square is occupied, so the pawn can't move there
        } else {
            moves |= (1ULL << to);  // Add the move to the bitboard
        }
    }

    if ((color == PIECE_COLOR_WHITE && from >= 8 && from <= 15) || (color == PIECE_COLOR_BLACK && from >= 48 && from <= 55)) {
        to = from + direction * 16;
        if (to >= 0 && to < 64) {
            // Make sure the two squares ahead are empty
            if (((board->bitboards[INDEX_BLACK_PAWN] | board->bitboards[INDEX_WHITE_PAWN]) & (1ULL << (from + direction * 8))) == 0 &&
                ((board->bitboards[INDEX_BLACK_PAWN] | board->bitboards[INDEX_WHITE_PAWN]) & (1ULL << to)) == 0) {
                moves |= (1ULL << to);  // Add the double move to the bitboard
            }
        }
    }

    // Capture moves (diagonal moves)
    int captureOffsets[] = {-9, -7, 7, 9};  // Capture diagonally
    for (int i = 0; i < 4; i++) {
        to = from + captureOffsets[i];
        if (to >= 0 && to < 64) {
            // Make sure the square is occupied by an opponent's piece
            if ((color == PIECE_COLOR_WHITE && (board->bitboards[INDEX_BLACK_PAWN] & (1ULL << to))) ||
                (color == PIECE_COLOR_BLACK && (board->bitboards[INDEX_WHITE_PAWN] & (1ULL << to)))) {
                moves |= (1ULL << to);  // Add the capture move to the bitboard
            }
        }
    }

    return moves;
}

void GenerateLegalMoves(const Board* board, uint8_t color, Move* moves, size_t* move_count)
{
    *move_count = 0;
    
    uint64_t piecesBitboard = (color == PIECE_COLOR_WHITE) ? GetWhite(board) : GetBlack(board);

    for (size_t i = 0; i < 64; i++) {
        if (piecesBitboard & (1ULL << i)) {
            Square from = (Square)i;
            Piece piece = PieceAt(board, from);

            Bitboard possibleMoves = 0;
            
            switch (piece.type) {
                case 'P': // White Pawn
                case 'p': // Black Pawn
                    possibleMoves = GeneratePawnMoves(board, from, piece.color);
                    break;
                case 'N': // White Knight
                case 'n': // Black Knight
                    possibleMoves = GenerateKnightMoves(board, from);
                    break;
                case 'B': // White Bishop
                case 'b': // Black Bishop
                    possibleMoves = GenerateBishopMoves(board, from);
                    break;
                case 'R': // White Rook
                case 'r': // Black Rook
                    possibleMoves = GenerateRookMoves(board, from);
                    break;
                case 'Q': // White Queen
                case 'q': // Black Queen
                    possibleMoves = GenerateQueenMoves(board, from);
                    break;
                case 'K': // White King
                case 'k': // Black King
                    possibleMoves = GenerateKingMoves(board, from);
                    break;
                default:
                    continue; // Ignore invalid pieces
            }

            for (size_t j = 0; j < 64; j++) {
                if (possibleMoves & (1ULL << j)) {
                    Square to = (Square)j;

                    // Temporarily make the move
                    Board tempBoard = *board;
                    MoveFreely(&tempBoard, from, to);

                    // Check if the move puts the king in check
                    if (IsKingInCheck(tempBoard, color)) {
                        continue; // Skip illegal move
                    }

                    // Add legal move to the list
                    moves[*move_count] = MoveEncode(from, to, PROMOTION_NONE, FLAG_NORMAL);
                    (*move_count)++;
                }
            }
        }
    }
}
