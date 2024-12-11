#ifndef BOARD_H
#define BOARD_H

#include "common.h"
#include "hashing.h"
#include "square.h"
#include <stdint.h>

/*------------------------------------------.
| 2D character array representation of the  |
| chess board. All methods and types follow | 
| the snake_case for differentiation        |
| from the below methodology                |
`------------------------------------------*/

#define BLACK_ROOK 'r'
#define BLACK_KNIGHT 'n'
#define BLACK_BISHOP 'b'
#define BLACK_KING 'k'
#define BLACK_QUEEN 'q'
#define BLACK_PAWN 'p'

#define WHITE_ROOK 'R'
#define WHITE_KNIGHT 'N'
#define WHITE_BISHOP 'B'
#define WHITE_KING 'K'
#define WHITE_QUEEN 'Q'
#define WHITE_PAWN 'P'

#define EMPTY_SQUARE ' '

#define STARTING_FEN "rnbqkbnr/pppppppp/9/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"

typedef struct {
    char grid[8][8];
    state_t state;
    char enpassant_square[2 + 1]; // +1 for null terminator
} board_t;

void board_init_fen(board_t* board, const char* fen);
void board_init_board(board_t* board, board_t src);

_Bool square_is_attacked(const board_t* board, square_t square, int attacked_by);
_Bool square_is_attacked_coords(const board_t *board, int y, int x, int attacked_by);
_Bool square_is_attacked_fr(const board_t *board, int rank, int file, int attacked_by);
square_t** square_is_attacked_by(const board_t* board, square_t square, int attacked_by, size_t* count);
square_t** square_is_accessible_by(const board_t* board, square_t square, char piece, size_t* count);
square_t** attack_path_to_king(board_t* board, square_t king, square_t attacker, size_t* path_count);

void find_king(square_t* square, const board_t* board, int color);

_Bool in_check(const board_t* board, int color);
#define IN_CHECK(board, color) \
    in_check(board, color)

char* update_enpassant_square(board_t* board, square_t from, square_t to);
uint8_t update_castling_rights(board_t* board, square_t from);
void update_halfmove(board_t* board, square_t from, square_t to, size_t piece_count_before, size_t piece_count_after, char piece);

size_t number_of_pieces(const board_t* board, int color);

_Bool is_checkmate(board_t* board);
_Bool is_stalemate(board_t* board);
_Bool is_insufficient_material(board_t* board);
_Bool is_threefold_repetition(board_t* board);


/*--------------------------------------------.
| Bitboard representation of the chess board. |
| All methods and types follow the PascalCase | 
| for differentiation from the above          |
| methodology                                 |
`--------------------------------------------*/

#define PIECE_TYPE_COUNT 12

typedef uint64_t Bitboard;

typedef struct {
    Bitboard bitboards[PIECE_TYPE_COUNT];
    Square enpassant_square;
    state_t state;
} Board;

#define PIECES "pnbrqkPNBRQK"
enum {
    INDEX_BLACK_PAWN,
    INDEX_BLACK_KNIGHT,
    INDEX_BLACK_BISHOP,
    INDEX_BLACK_ROOK,
    INDEX_BLACK_QUEEN,
    INDEX_BLACK_KING,
    INDEX_WHITE_PAWN,
    INDEX_WHITE_KNIGHT,
    INDEX_WHITE_BISHOP,
    INDEX_WHITE_ROOK,
    INDEX_WHITE_QUEEN,
    INDEX_WHITE_KING,
};

typedef enum {
    FLAG_NORMAL = 0,
    FLAG_CASTLING,
    FLAG_ENPASSANT,
    FLAG_PAWN_DOUBLE_MOVE,
    FLAG_PROMOTION,
    FLAG_PROMOTION_WITH_CAPTURE,
} Flag;

typedef enum {
    PROMOTION_NONE = 0,
    PROMOTION_QUEEN,
    PROMOTION_ROOK,
    PROMOTION_BISHOP,
    PROMOTION_KNIGHT
} Promotion;

char PromotionToChar(uint8_t promotion);
uint8_t CharToPromotion(char promotion);

void BoardInitFen(Board* board, const char* fen);
void BoardPrint(Board board);
void BitboardPrint(Board board, int index);

void Uint32Print(uint32_t value);
void Uint64Print(uint64_t value);

_Bool IsSquareAttacked(Board board, Square square, uint8_t color);
_Bool IsKingInCheck(Board board, uint8_t color);


#endif // BOARD_H
