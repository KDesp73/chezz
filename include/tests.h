#ifndef TESTS_H
#define TESTS_H

#include "board.h"
#include <stddef.h>
int test_square_from_name(const char* name, size_t expected_x, size_t expected_y);
#define TEST_SQUARE_FROM_NAME \
    test_square_from_name("a1", 0, 0), \
    test_square_from_name("b1", 1, 0), \
    test_square_from_name("c1", 2, 0), \
    test_square_from_name("d1", 3, 0), \
    test_square_from_name("e1", 4, 0), \
    test_square_from_name("f1", 5, 0), \
    test_square_from_name("g1", 6, 0), \
    test_square_from_name("h1", 7, 0), \
    test_square_from_name("a1", 0, 0), \
    test_square_from_name("b2", 1, 1), \
    test_square_from_name("c3", 2, 2), \
    test_square_from_name("d4", 3, 3), \
    test_square_from_name("e5", 4, 4), \
    test_square_from_name("f6", 5, 5), \
    test_square_from_name("g7", 6, 6), \
    test_square_from_name("h8", 7, 7), \
    test_square_from_name("a2", 0, 1), \
    test_square_from_name("a3", 0, 2)

int test_pawn_can_move(const char* fen, const char* from, const char* to, _Bool expected);
#define TEST_PAWN_MOVE \
    test_pawn_can_move(STARTING_FEN, "c2", "d3", 0), \
    test_pawn_can_move(STARTING_FEN, "a2", "a3", 1), \
    test_pawn_can_move(STARTING_FEN, "a2", "a4", 1), \
    test_pawn_can_move(STARTING_FEN, "e2", "e4", 1), \
    test_pawn_can_move(STARTING_FEN, "a2", "a5", 0), \
    test_pawn_can_move(STARTING_FEN, "h7", "h6", 1), \
    test_pawn_can_move(STARTING_FEN, "h7", "h5", 1), \
    test_pawn_can_move("rnbqkbnr/ppppp1pp/8/8/8/5p2/PPPPPPPP/RNBQKBNR w KQkq - 0 1", "e2", "f3", 1), \
    test_pawn_can_move("rnbqkbnr/ppppp1pp/8/8/8/5p2/PPPPPPPP/RNBQKBNR w KQkq - 0 1", "g2", "f3", 1), \
    test_pawn_can_move("rnbqkbnr/ppppp1pp/8/8/8/5p2/PPPPPPPP/RNBQKBNR w KQkq - 0 1", "f2", "f3", 0), \
    test_pawn_can_move("rnbqkbnr/ppppp1pp/8/8/5p2/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1", "f2", "f3", 1), \
    test_pawn_can_move("rnbqkbnr/ppppp1pp/8/8/5p2/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1", "f2", "f4", 0), \
    test_pawn_can_move("rnbqkbnr/ppppp1pp/8/8/8/5p2/PPPPPPPP/RNBQKBNR w KQkq - 0 1", "f2", "f4", 0), \
    test_pawn_can_move("rnbqkbnr/pppppppp/4P3/8/8/8/PPPP1PPP/RNBQKBNR w KQkq - 0 1", "f7", "e6", 1), \
    test_pawn_can_move("rnbqkbnr/pppppppp/4P3/8/8/8/PPPP1PPP/RNBQKBNR w KQkq - 0 1", "d7", "e6", 1), \
    test_pawn_can_move("rnbqkbnr/pppppppp/4P3/8/8/8/PPPP1PPP/RNBQKBNR w KQkq - 0 1", "e7", "e6", 0), \
    test_pawn_can_move("rnbqkbnr/ppp1pppp/8/8/3p4/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1", "d4", "e2", 0), \
    test_pawn_can_move("rnbqkbnr/pppppppp/4P3/8/8/8/PPPP1PPP/RNBQKBNR w KQkq - 0 1", "e7", "e5", 0), \
    test_pawn_can_move("rnbqkbnr/ppp1pppp/8/2PpP3/8/8/PP1P1PPP/RNBQKBNR w KQkq d6 0 1", "e5", "d6", 1), \
    test_pawn_can_move("rnbqkbnr/ppp1pppp/8/2PpP3/8/8/PP1P1PPP/RNBQKBNR w KQkq d6 0 1", "c5", "d6", 1)

int test_rook_can_move(const char* fen, const char* from, const char* to, _Bool expected);
#define TEST_ROOK_MOVE \
    test_rook_can_move(STARTING_FEN, "a1", "a2", 0), \
    test_rook_can_move(STARTING_FEN, "a1", "a3", 0), \
    test_rook_can_move(STARTING_FEN, "a1", "a3", 0), \
    test_rook_can_move("rnbqkbnr/pppppppp/8/8/5P2/8/1PPPPPPP/RNBQKBNR w KQkq - 0 1", "a1", "a2", 1), \
    test_rook_can_move("rnbqkbnr/pppppppp/8/8/5P2/8/1PPPPPPP/RNBQKBNR w KQkq - 0 1", "a1", "a3", 1), \
    test_rook_can_move("rnbqkbnr/pppppppp/8/8/5P2/8/1PPPPPPP/RNBQKBNR w KQkq - 0 1", "a1", "a4", 1), \
    test_rook_can_move("rnbqkbnr/pppppppp/8/8/5P2/8/1PPPPPPP/RNBQKBNR w KQkq - 0 1", "a1", "a5", 1), \
    test_rook_can_move("rnbqkbnr/pppppppp/8/8/5P2/8/1PPPPPPP/RNBQKBNR w KQkq - 0 1", "a1", "a6", 1), \
    test_rook_can_move("rnbqkbnr/pppppppp/8/8/5P2/8/1PPPPPPP/RNBQKBNR w KQkq - 0 1", "a1", "a7", 1), \
    test_rook_can_move("rnbqkbnr/pppppppp/8/8/5P2/8/1PPPPPPP/RNBQKBNR w KQkq - 0 1", "a1", "a8", 0), \
    test_rook_can_move("8/8/8/3Rr3/8/8/8/8 w - - 0 1", "d5", "d6", 1), \
    test_rook_can_move("8/8/8/3Rr3/8/8/8/8 w - - 0 1", "e5", "d6", 0), \
    test_rook_can_move("8/8/8/3Rr3/8/8/8/8 w - - 0 1", "d5", "e5", 1), \
    test_rook_can_move("8/8/8/3Rr3/8/8/8/8 w - - 0 1", "e5", "d5", 1), \
    test_rook_can_move("8/8/8/3Rr3/8/8/8/8 w - - 0 1", "d5", "f5", 0), \
    test_rook_can_move("8/8/8/3Rr3/8/8/8/8 w - - 0 1", "e5", "c5", 0), \
    test_rook_can_move("R6r/8/8/8/8/8/8/8 w - - 0 1", "a8", "h8", 1), \
    test_rook_can_move("R6r/8/8/8/8/8/8/8 w - - 0 1", "a8", "a1", 1), \
    test_rook_can_move("R6r/8/8/8/8/8/8/8 w - - 0 1", "a8", "h1", 0)

int test_bishop_can_move(const char* fen, const char* from, const char* to, _Bool expected);
#define TEST_BISHOP_MOVE \
    test_bishop_can_move(STARTING_FEN, "c1", "d2", 0), \
    test_bishop_can_move(STARTING_FEN, "c1", "b2", 0), \
    test_bishop_can_move(STARTING_FEN, "c1", "c3", 0), \
    test_bishop_can_move("rnbqkbnr/pppppppp/8/8/8/3P4/PPP1PPPP/RNBQKBNR w KQkq - 0 1", "c1", "d2", 1), \
    test_bishop_can_move("rnbqkbnr/pppppppp/8/8/8/3P4/PPP1PPPP/RNBQKBNR w KQkq - 0 1", "c1", "e3", 1), \
    test_bishop_can_move("rnbqkbnr/pppppppp/8/8/8/3P4/PPP1PPPP/RNBQKBNR w KQkq - 0 1", "c1", "f4", 1), \
    test_bishop_can_move("rnbqkbnr/pppppppp/8/8/8/3P4/PPP1PPPP/RNBQKBNR w KQkq - 0 1", "c1", "g5", 1), \
    test_bishop_can_move("rnbqkbnr/pppppppp/8/8/8/3P4/PPP1PPPP/RNBQKBNR w KQkq - 0 1", "c1", "h6", 1), \
    test_bishop_can_move("rnbqkbnr/pppppppp/8/8/8/3P4/PPP1PPPP/RNBQKBNR w KQkq - 0 1", "c1", "a3", 0), \
    test_bishop_can_move("rnbqkbnr/ppppp1pp/8/8/5p2/3P4/PPP1PPPP/RNBQKBNR w KQkq - 0 1", "c1", "d2", 1), \
    test_bishop_can_move("rnbqkbnr/ppppp1pp/8/8/5p2/3P4/PPP1PPPP/RNBQKBNR w KQkq - 0 1", "c1", "e3", 1), \
    test_bishop_can_move("rnbqkbnr/ppppp1pp/8/8/5p2/3P4/PPP1PPPP/RNBQKBNR w KQkq - 0 1", "c1", "f4", 1), \
    test_bishop_can_move("rnbqkbnr/ppppp1pp/8/8/5p2/3P4/PPP1PPPP/RNBQKBNR w KQkq - 0 1", "c1", "g5", 0), \
    test_bishop_can_move("8/8/8/8/8/3BB3/8/8 w - - 0 1", "d3", "h7", 1), \
    test_bishop_can_move("8/8/8/8/8/3BB3/8/8 w - - 0 1", "d3", "e3", 0), \
    test_bishop_can_move("8/8/8/8/8/3BB3/8/8 w - - 0 1", "d3", "g5", 0)

int test_queen_can_move(const char* fen, const char* from, const char* to, _Bool expected);
#define TEST_QUEEN_MOVE \
    test_queen_can_move(STARTING_FEN, "d1", "c2", 0), \
    test_queen_can_move(STARTING_FEN, "d1", "d2", 0), \
    test_queen_can_move(STARTING_FEN, "d1", "e2", 0), \
    test_queen_can_move(STARTING_FEN, "d1", "d3", 0), \
    test_queen_can_move(STARTING_FEN, "d1", "c3", 0), \
    test_queen_can_move(STARTING_FEN, "d1", "e3", 0), \
    test_queen_can_move("rnbqkbnr/pppppppp/8/8/3Q4/8/PPPPPPPP/RNB1KBNR w KQkq - 0 1", "d4", "h4", 1), \
    test_queen_can_move("rnbqkbnr/pppppppp/8/8/3Q4/8/PPPPPPPP/RNB1KBNR w KQkq - 0 1", "d4", "a4", 1), \
    test_queen_can_move("rnbqkbnr/pppppppp/8/8/3Q4/8/PPPPPPPP/RNB1KBNR w KQkq - 0 1", "d4", "c3", 1), \
    test_queen_can_move("rnbqkbnr/pppppppp/8/8/3Q4/8/PPPPPPPP/RNB1KBNR w KQkq - 0 1", "d4", "e3", 1), \
    test_queen_can_move("rnbqkbnr/pppppppp/8/8/3Q4/8/PPPPPPPP/RNB1KBNR w KQkq - 0 1", "d4", "d3", 1), \
    test_queen_can_move("rnbqkbnr/pppppppp/8/8/3Q4/8/PPPPPPPP/RNB1KBNR w KQkq - 0 1", "d4", "d6", 1), \
    test_queen_can_move("rnbqkbnr/pppppppp/8/8/3Q4/8/PPPPPPPP/RNB1KBNR w KQkq - 0 1", "d4", "d7", 1), \
    test_queen_can_move("rnbqkbnr/pppppppp/8/8/3Q4/8/PPPPPPPP/RNB1KBNR w KQkq - 0 1", "d4", "f6", 1), \
    test_queen_can_move("rnbqkbnr/pppppppp/8/8/3Q4/8/PPPPPPPP/RNB1KBNR w KQkq - 0 1", "d4", "g7", 1), \
    test_queen_can_move("rnbqkbnr/pppppppp/8/8/3Q4/8/PPPPPPPP/RNB1KBNR w KQkq - 0 1", "d4", "a7", 1), \
    test_queen_can_move("rnbqkbnr/pppppppp/8/8/3Q4/8/PPPPPPPP/RNB1KBNR w KQkq - 0 1", "d4", "b6", 1), \
    test_queen_can_move("rnbqkbnr/pppppppp/8/8/3Q4/8/PPPPPPPP/RNB1KBNR w KQkq - 0 1", "d4", "d1", 0), \
    test_queen_can_move("rnbqkbnr/pppp1ppp/8/4p3/3Q4/8/PPPPPPPP/RNB1KBNR w KQkq - 0 1", "d4", "f6", 0)

int test_knight_can_move(const char* fen, const char* from, const char* to, _Bool expected);
#define TEST_KNIGHT_MOVE \
    test_knight_can_move(STARTING_FEN, "b1", "c3", 1), \
    test_knight_can_move(STARTING_FEN, "b1", "a3", 1), \
    test_knight_can_move(STARTING_FEN, "b1", "d2", 0), \
    test_knight_can_move(STARTING_FEN, "g1", "f3", 1), \
    test_knight_can_move(STARTING_FEN, "g1", "h3", 1), \
    test_knight_can_move(STARTING_FEN, "g1", "e2", 0), \
    test_knight_can_move("rnbqkbnr/pppppppp/8/4N3/8/8/PPPPPPPP/RNBQKB1R w KQkq - 0 1", "e5", "d7", 1), \
    test_knight_can_move("rnbqkbnr/pppppppp/8/4N3/8/8/PPPPPPPP/RNBQKB1R w KQkq - 0 1", "e5", "f7", 1), \
    test_knight_can_move("rnbqkbnr/pppppppp/8/4N3/8/8/PPPPPPPP/RNBQKB1R w KQkq - 0 1", "e5", "c6", 1), \
    test_knight_can_move("rnbqkbnr/pppppppp/8/4N3/8/8/PPPPPPPP/RNBQKB1R w KQkq - 0 1", "e5", "c4", 1), \
    test_knight_can_move("rnbqkbnr/pppppppp/8/4N3/8/8/PPPPPPPP/RNBQKB1R w KQkq - 0 1", "e5", "d3", 1), \
    test_knight_can_move("rnbqkbnr/pppppppp/8/4N3/8/8/PPPPPPPP/RNBQKB1R w KQkq - 0 1", "e5", "f3", 1), \
    test_knight_can_move("rnbqkbnr/pppppppp/8/4N3/8/8/PPPPPPPP/RNBQKB1R w KQkq - 0 1", "e5", "g4", 1), \
    test_knight_can_move("rnbqkbnr/pppppppp/8/4N3/8/8/PPPPPPPP/RNBQKB1R w KQkq - 0 1", "e5", "g6", 1), \
    test_knight_can_move("rnbqkbnr/pppppppp/8/4N3/8/8/PPPPPPPP/RNBQKB1R w KQkq - 0 1", "e5", "a1", 0)

int test_king_can_move(const char* fen, const char* from, const char* to, _Bool expected);
#define TEST_KING_MOVE \
    test_king_can_move(STARTING_FEN, "e1", "e2", 0), \
    test_king_can_move("8/8/3k4/8/3K4/8/8/8 w - - 0 1", "d4", "d3", 1), \
    test_king_can_move("8/8/3k4/8/3K4/8/8/8 w - - 0 1", "d4", "c3", 1), \
    test_king_can_move("8/8/3k4/8/3K4/8/8/8 w - - 0 1", "d4", "e3", 1), \
    test_king_can_move("8/8/3k4/8/3K4/8/8/8 w - - 0 1", "d4", "e4", 1), \
    test_king_can_move("8/8/3k4/8/3K4/8/8/8 w - - 0 1", "d4", "c4", 1), \
    test_king_can_move("8/8/3k4/8/3K4/8/8/8 w - - 0 1", "d4", "d5", 0), \
    test_king_can_move("8/8/3k4/8/3K4/8/8/8 w - - 0 1", "d4", "c5", 0), \
    test_king_can_move("8/8/3k4/8/3K4/8/8/8 w - - 0 1", "d4", "e5", 0), \
    test_king_can_move("r3k2r/8/8/8/8/8/8/R3K2R w KQkq - 0 1", "e1", "g1", 1), \
    test_king_can_move("r3k2r/8/8/8/8/8/8/R3K2R w KQkq - 0 1", "e1", "c1", 1), \
    test_king_can_move("r3k2r/8/8/8/8/8/8/R3K2R w KQkq - 0 1", "e8", "g8", 1), \
    test_king_can_move("r3k2r/8/8/8/8/8/8/R3K2R w KQkq - 0 1", "e8", "c8", 1), \
    test_king_can_move("r3k2r/8/8/8/8/8/8/R3K2R w - - 0 1", "e1", "g1", 0), \
    test_king_can_move("r3k2r/8/8/8/8/8/8/R3K2R w - - 0 1", "e1", "c1", 0), \
    test_king_can_move("r3k2r/8/8/8/8/8/8/R3K2R w - - 0 1", "e8", "g8", 0), \
    test_king_can_move("r3k2r/8/8/8/8/8/8/R3K2R w - - 0 1", "e8", "c8", 0), \
    test_king_can_move("3rkr2/8/8/8/8/8/8/R3K2R w KQkq - 0 1", "e1", "g1", 0), \
    test_king_can_move("3rkr2/8/8/8/8/8/8/R3K2R w KQkq - 0 1", "e1", "c1", 0), \
    test_king_can_move("2r1k1r1/8/8/8/8/8/8/R3K2R w KQkq - 0 1", "e1", "g1", 0), \
    test_king_can_move("2r1k1r1/8/8/8/8/8/8/R3K2R w KQkq - 0 1", "e1", "c1", 0), \
    test_king_can_move("r3k2r/8/8/8/8/8/1p5p/R3K2R w KQkq - 0 1", "e1", "g1", 0), \
    test_king_can_move("r3k2r/8/8/8/8/8/1p5p/R3K2R w KQkq - 0 1", "e1", "c1", 0), \
    test_king_can_move("r3k2r/8/8/8/8/8/2p3p1/R3K2R w KQkq - 0 1", "e1", "g1", 0), \
    test_king_can_move("r3k2r/8/8/8/8/8/2p3p1/R3K2R w KQkq - 0 1", "e1", "c1", 0), \
    test_king_can_move("r3k2r/2p3p1/8/8/8/2n3n1/8/R3K2R w KQkq - 0 1", "e1", "g1", 0), \
    test_king_can_move("r3k2r/2p3p1/8/8/8/2n3n1/8/R3K2R w KQkq - 0 1", "e1", "c1", 0), \
    test_king_can_move("r3k2r/2p3p1/8/8/8/1n5n/8/R3K2R w KQkq - 0 1", "e1", "g1", 0), \
    test_king_can_move("r3k2r/2p3p1/8/8/8/1n5n/8/R3K2R w KQkq - 0 1", "e1", "c1", 0), \
    test_king_can_move("rn2k1nr/2p3p1/8/8/8/4b3/8/R3K2R w KQkq - 0 1", "e1", "g1", 0), \
    test_king_can_move("rn2k1nr/2p3p1/8/8/8/4b3/8/R3K2R w KQkq - 0 1", "e1", "c1", 0), \
    test_king_can_move("rn2k1nr/2p3p1/8/8/8/8/4b3/R3K2R w KQkq - 0 1", "e1", "g1", 0), \
    test_king_can_move("rn2k1nr/2p3p1/8/8/8/8/4b3/R3K2R w KQkq - 0 1", "e1", "c1", 0), \
    test_king_can_move("rn2k1nr/2p3p1/8/8/8/8/4q3/R3K2R w KQkq - 0 1", "e1", "g1", 0), \
    test_king_can_move("rn2k1nr/2p3p1/8/8/8/8/4q3/R3K2R w KQkq - 0 1", "e1", "c1", 0), \
    test_king_can_move("rnbq2nr/2p3p1/8/8/8/8/6k1/R3K2R w KQ - 0 1", "e1", "g1", 0), \
    test_king_can_move("rnbq2nr/2p3p1/8/8/8/8/2k5/R3K2R w KQ - 0 1", "e1", "c1", 0), \
    test_king_can_move("rnbq2nr/3p2p1/8/8/8/8/2k5/R3K2R w KQha - 0 1", "e1", "c1", 0), \
    test_king_can_move("r3k2r/8/8/4q3/8/8/8/R3K2R w KQkq - 0 1", "e1", "c1", 0)

int test_is_pinned(const char* fen, const char* square, _Bool expected);
#define TEST_IS_PINNED \
    test_is_pinned("rnb1k1nr/pp1ppppp/2p5/4q3/6Pb/3P1P2/PPP2N1P/RNBQKB1R w KQkq - 0 1", "f2", 1), \
    test_is_pinned("rnb1k1nr/pp1ppppp/2p5/4q3/6Pb/3PBP2/PPP2N1P/RN1QKB1R w KQkq - 0 1", "f2", 1), \
    test_is_pinned("rnb1k1nr/pp1ppppp/2p5/4q3/6Pb/3PBP2/PPP2N1P/RN1QKB1R w KQkq - 0 1", "e3", 1), \
    test_is_pinned("rnb1k1nr/pp1ppppp/2p5/4q3/6Pb/3PBP2/PPP1BN1P/RN1QK2R w KQkq - 0 1", "e3", 0), \
    test_is_pinned("rnb1kbnr/pppp1ppp/8/4p3/5P1q/4P1P1/PPPP3P/RNBQKBNR w KQkq - 0 1", "g3", 1), \
    test_is_pinned("rnb1kbnr/pppp1ppp/8/4p3/5P1q/4P1P1/PPPP3P/RNBQKBNR w KQkq - 0 1", "f4", 0), \
    test_is_pinned("rnb1kbnr/pppp1ppp/8/4p3/2K2P1q/4P1P1/PPPP3P/RNBQ1BNR w kq - 0 1", "f4", 1), \
    test_is_pinned("rn2kbnr/pppp1ppp/4b3/3Pp3/2K2P1q/4P1P1/PPP4P/RNBQ1BNR w kq - 0 1", "d5", 1), \
    test_is_pinned("r3kbnr/pppp1ppp/4b3/3Pp3/4nP1q/4P1P1/PPP2K1P/RNBQ1BNR w kq - 0 1", "g3", 1), \
    test_is_pinned("r3kbnr/pppp1ppp/4b3/3Pp2q/4nP2/4P1P1/PPP2K1P/RNBQ1BNR w kq - 0 1", "g3", 0), \
    test_is_pinned("r3kbnr/pppp1ppp/4b3/3Pp2q/4nP2/4P1P1/PPP2K1P/RNBQ1BNR w kq - 0 1", "a1", 0), \
    test_is_pinned("r3kbnr/pppp1ppp/4b3/3Pp2q/4nP2/4P1P1/PPP2K1P/RNBQ1BNR w kq - 0 1", "b1", 0), \
    test_is_pinned("r3kbnr/pppp1ppp/4b3/1B1Pp2q/4nP2/4P1P1/PPP4P/RNBQK1NR w kq - 0 1", "d7", 1), \
    test_is_pinned("r3kbnr/ppNp1ppp/4b3/1B1Pp2q/4nP2/4P1P1/PPP4P/R1BQK1NR w kq - 0 1", "d7", 1), \
    test_is_pinned("r3kbnr/ppNp1ppp/4b3/1B1Pp2q/4nP2/4P1P1/PPP4P/R1BQK1NR w kq - 0 1", "a8", 0), \
    test_is_pinned("r3kbnr/ppNp1ppp/4b3/1B1Pp2q/4nP2/4P1P1/PPP4P/R1BQK1NR w kq - 0 1", "e6", 0), \
    test_is_pinned("r1bk1bnr/pp4pp/2npNp2/4P1B1/3P4/5N2/PPP3PP/R2QKB1R b KQ - 0 11", "f6", 1), \
    test_is_pinned("r1b2bnr/pp2k1pp/2npNP2/6B1/3P4/5N2/PPP3PP/R2QKB1R b KQ - 0 12", "f6", 0)

int test_valid_moves(const char* fen, const char* square, const char* first, ...);
#define TEST_VALID_MOVES \
    test_valid_moves("r3kbnr/ppNp1ppp/4b3/1B1Pp2q/4nP2/4P1P1/PPP4P/R1BQK1NR w kq - 0 1", "a1", "b1", NULL), \
    test_valid_moves("r3kbnr/ppNp1ppp/4b3/1B1Pp2q/4nP2/4P1P1/PPP4P/R1BQK1NR w kq - 0 1", "a2", "a3", "a4", NULL), \
    test_valid_moves("r3kbnr/ppNp1ppp/4b3/1B1Pp2q/4nP2/4P1P1/PPP4P/R1BQK1NR w kq - 0 1", "c1", "d2", NULL), \
    test_valid_moves("r3kbnr/ppNp1ppp/4b3/1B1Pp2q/4nP2/4P1P1/PPP4P/R1BQK1NR w kq - 0 1", "d1", "d2", "d3", "d4", "e2", "f3", "g4", "h5", NULL), \
    test_valid_moves("r3kbnr/ppNp1ppp/4b3/1B1Pp2q/4nP2/4P1P1/PPP4P/R1BQK1NR w kq - 0 1", "f4", "e5", "f5", NULL), \
    test_valid_moves("r3kbnr/ppNp1ppp/4b3/1B1Pp2q/4nP2/4P1P1/PPP4P/R1BQK1NR w kq - 0 1", "f7", "f6", "f5", NULL), \
    test_valid_moves("r3kbnr/ppNp1ppp/4b3/1B1Pp2q/4nP2/4P1P1/PPP4P/R1BQK1NR w kq - 0 1", "c7", "a8", "a6", "e6", NULL), \
    test_valid_moves("r3kbnr/ppNp1ppp/4b3/1B1Pp2q/4nP2/4P1P1/PPP4P/R1BQK1NR w kq - 0 1", "b5", "a4", "c4", "d3", "e2", "f1", "c6", "d7", "a6", NULL), \
    test_valid_moves("r3kb1r/ppB1n1pp/5p2/8/3P2b1/2N4P/PPP1B1P1/2KR3R w Kkq - 0 1", "e8", "f7", "d7", NULL), \
    test_valid_moves("r3kb1r/ppB1n1pp/5p2/8/3P2b1/2N4P/PPP1B1P1/2KR3R w Kkq - 0 1", "a7", "a6", "a5", NULL), \
    test_valid_moves("r3kb1r/ppB1n1pp/5p2/8/3P2b1/2N4P/PPP1B1P1/2KR3R w Kkq - 0 1", "f6", "f5", NULL), \
    test_valid_moves("r3kb1r/ppB1n1pp/5p2/8/3P2b1/2N4P/PPP1B1P1/2KR3R w Kkq - 0 1", "g4", "h3", "f3", "e2", "h5", "f5", "e6", "d7", "c8", NULL), \
    test_valid_moves("r3kb1r/ppB1n1pp/5p2/8/3P2b1/2N4P/PPP1B1P1/2KR3R w Kkq - 0 1", "c3", "a4", "b5", "d5", "e4", "b1", NULL), \
    test_valid_moves("r3kb1r/pp2n1pp/5p2/8/3P2b1/1BN4P/PPP1B1P1/2KR3R w Kkq - 0 1", "e8", "d7", "d8", "c8", NULL), \
    test_valid_moves("rnb1kbnr/ppppqppp/8/2p5/2P5/8/PPPPQPPP/RNB1KBNR w KQkq - 0 1", "e2", "e3", "e4", "e5", "e6", "e7", NULL), \
    test_valid_moves("3k4/8/6b1/8/8/3B4/2K5/8 w - - 0 1", "d3", "e4", "f5", "g6", NULL), \
    test_valid_moves("3kr3/8/8/4R3/8/8/4K3/8 w - - 0 1", "e5", "e3", "e4", "e6", "e7", "e8", NULL), \
    test_valid_moves("rnbqkbnr/1p2pppp/p2p4/8/8/8/PPPPKPPP/RNBQ1BNR b kq - 0 1", "e8", "d7", NULL)

int test_move_is_valid(const char* fen, const char* from, const char* to, _Bool expected);
#define TEST_MOVE_IS_VALID \
    test_move_is_valid("r1bqkb1r/pppp1ppp/2n2n2/4p3/2B1P3/5N2/PPPP1PPP/RNBQK2R w KQkq - 0 1", "a2", "a3", 1), \
    test_move_is_valid("r1bqkb1r/pppp1ppp/2n2n2/4p3/2B1P3/5N2/PPPP1PPP/RNBQK2R b KQkq - 0 1", "f6", "e4", 1), \
    test_move_is_valid("r1bqkb1r/pppp1ppp/2n5/3np2Q/2B1P3/5N2/PPPP1PPP/RNB1K2R b KQkq - 0 1", "f7", "f6", 0), \
    test_move_is_valid("r1bqkb1r/pppp2pp/2n2p2/3np2Q/2B1P3/5N2/PPPP1PPP/RNB1K2R b KQkq - 0 1", "a8", "b8", 0), \
    test_move_is_valid("r1bqkb1r/pppp2pp/2n2p2/3np2Q/2B1P3/5N2/PPPP1PPP/RNB1K2R b KQkq - 0 1", "a7", "a6", 0), \
    test_move_is_valid("r1bqkb1r/pppp2pp/2n2p2/3np2Q/2B1P3/5N2/PPPP1PPP/RNB1K2R b KQkq - 0 1", "e8", "e7", 1), \
    test_move_is_valid("r1bqkb1r/pppp2pp/2n2p2/3np2Q/2B1P3/5N2/PPPP1PPP/RNB1K2R b KQkq - 0 1", "g7", "g6", 1), \
    test_move_is_valid("r1bqkb1r/pppp2pp/2n2p2/3np2Q/2B1P3/5N2/PPPP1PPP/RNB1K2R w KQkq - 0 1", "e1", "g1", 1), \
    test_move_is_valid("r1bqkb1r/pppp2pp/2n2p2/3np2Q/2B1P3/5N2/PPPP1PPP/RNB1K2R w Qkq - 0 1", "e1", "g1", 0), \
    test_move_is_valid("r1bqkb1r/pppp2pp/2n2p2/3np2Q/2B1P3/5N2/PPPP1PPP/RNB1K2R w Qkq - 0 1", "e1", "g1", 0), \
    test_move_is_valid("rnb1kbnr/ppppqppp/8/2p5/2P5/8/PPPPQPPP/RNB1KBNR w KQkq - 0 1", "e2", "d1", 0), \
    test_move_is_valid("rnb1kbnr/ppppqppp/8/2p5/2P5/8/PPPPQPPP/RNB1KBNR w KQkq - 0 1", "e2", "e3", 1), \
    test_move_is_valid("rnb1kbnr/ppppqppp/8/2p5/2P5/8/PPPPQPPP/RNB1KBNR w KQkq - 0 1", "e2", "e4", 1), \
    test_move_is_valid("rnb1kbnr/ppppqppp/8/2p5/2P5/8/PPPPQPPP/RNB1KBNR w KQkq - 0 1", "e2", "e5", 1), \
    test_move_is_valid("rnb1kbnr/ppppqppp/8/2p5/2P5/8/PPPPQPPP/RNB1KBNR w KQkq - 0 1", "e2", "e6", 1), \
    test_move_is_valid("rnb1kbnr/ppppqppp/8/2p5/2P5/8/PPPPQPPP/RNB1KBNR w KQkq - 0 1", "e2", "e7", 1), \
    test_move_is_valid("3k4/8/6b1/8/8/3B4/2K5/8 w - - 0 1", "d3", "c4", 0), \
    test_move_is_valid("3k4/8/6b1/8/8/3B4/2K5/8 w - - 0 1", "d3", "e4", 1), \
    test_move_is_valid("3k4/8/6b1/8/8/3B4/2K5/8 w - - 0 1", "d3", "f5", 1), \
    test_move_is_valid("3k4/8/6b1/8/8/3B4/2K5/8 w - - 0 1", "d3", "g6", 1), \
    test_move_is_valid("3kr3/8/8/4R3/8/8/4K3/8 w - - 0 1", "e5", "d5", 0), \
    test_move_is_valid("3kr3/8/8/4R3/8/8/4K3/8 w - - 0 1", "e5", "e3", 1), \
    test_move_is_valid("3kr3/8/8/4R3/8/8/4K3/8 w - - 0 1", "e5", "e4", 1), \
    test_move_is_valid("3kr3/8/8/4R3/8/8/4K3/8 w - - 0 1", "e5", "e6", 1), \
    test_move_is_valid("3kr3/8/8/4R3/8/8/4K3/8 w - - 0 1", "e5", "e7", 1), \
    test_move_is_valid("3kr3/8/8/4R3/8/8/4K3/8 w - - 0 1", "e5", "e8", 1), \
    test_move_is_valid("rnbqkbnr/1p2pppp/p2p4/8/8/8/PPPPKPPP/RNBQ1BNR b kq - 0 1", "e8", "d7", 1), \
    test_move_is_valid("rnbqkbnr/pppp1pp1/8/4p3/4P1Pp/PP6/2PP1P1P/RNBQKBNR b KQkq g3 0 1", "h4", "g3", 1)


int test_pawn_is_enpassanting(const char* fen, const char* from, const char* to, _Bool expected);
#define TEST_PAWN_IS_ENPASSANTING \
    test_pawn_is_enpassanting("rnbqkbnr/1pppp1pp/p7/4Pp2/8/8/PPPP1PPP/RNBQKBNR w KQkq f6 0 1", "e5", "f6", 1), \
    test_pawn_is_enpassanting("rnbqkbnr/1pp1pppp/p7/3pP3/8/8/PPPP1PPP/RNBQKBNR w KQkq d6 0 1", "e5", "d6", 1), \
    test_pawn_is_enpassanting(STARTING_FEN, "b8", "c6", 0), \
    test_pawn_is_enpassanting("rnbqkbnr/pppp1pp1/8/4p3/4P1Pp/PP6/2PP1P1P/RNBQKBNR b KQkq g3 0 1", "h4", "g3", 1)

int test_pawn_can_enpassant(const char* fen, const char* from, const char* to, _Bool expected);
#define TEST_PAWN_CAN_ENPASSANT \
    test_pawn_can_enpassant("rnbqkbnr/1pppp1pp/p7/4Pp2/8/8/PPPP1PPP/RNBQKBNR w KQkq f6 0 1", "e5", "f6", 1), \
    test_pawn_can_enpassant("rnbqkbnr/1pp1pppp/p7/3pP3/8/8/PPPP1PPP/RNBQKBNR w KQkq d6 0 1", "e5", "d6", 1), \
    test_pawn_can_enpassant("rnbqkbnr/pppp1pp1/8/4p3/4P1Pp/PP6/2PP1P1P/RNBQKBNR b KQkq g3 0 1", "h4", "g3", 1)


int test_move(const char* fen, const char* from, const char* to, _Bool expected);
#define TEST_MOVE \
    test_move("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1", "e2", "e4", 1), \
    test_move("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1", "e2", "e3", 1), \
    test_move("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR b KQkq - 0 1", "e7", "e6", 1), \
    test_move("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR b KQkq - 0 1", "e7", "e5", 1), \
    test_move("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1", "g1", "f3", 1), \
    test_move("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1", "b1", "c3", 1), \
    test_move("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR b KQkq - 0 1", "g8", "f6", 1), \
    test_move("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR b KQkq - 0 1", "b8", "c6", 1), \
    test_move("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1", "a1", "a2", 0), \
    test_move("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1", "a1", "a3", 0), \
    test_move("rnbqkbnr/1pp1pppp/p7/3pP3/8/8/PPPP1PPP/RNBQKBNR w KQkq d6 0 1", "e5", "d6", 1), \
    test_move("rnbqkbnr/1pp1pppp/p7/3pP3/8/8/PPPP1PPP/RNBQKBNR w KQkq - 0 1", "e5", "d6", 0), \
    test_move("rnbqkbnr/pppp1pp1/8/4p3/4P1Pp/PP6/2PP1P1P/RNBQKBNR b KQkq g3 0 1", "h4", "g3", 1), \
    test_move("r3k2r/8/8/8/8/8/8/R3K2R w KQkq - 0 1", "e1", "g1", 1), \
    test_move("r3k2r/8/8/8/8/8/8/R3K2R w KQkq - 0 1", "e1", "c1", 1), \
    test_move("r3k2r/8/8/8/8/8/8/R3K2R b KQkq - 0 1", "e8", "g8", 1), \
    test_move("r3k2r/8/8/8/8/8/8/R3K2R b KQkq - 0 1", "e8", "c8", 1), \
    test_move("3rk2r/8/8/8/8/8/8/R3K2R w KQkq - 0 1", "e1", "c1", 0), \
    test_move("3rk2r/8/8/8/8/8/8/R3K2R w KQkq - 0 1", "e1", "g1", 1), \
    test_move("3rk2r/8/8/8/8/8/8/R3K2R w KQkq - 0 1", "e1", "d1", 0), \
    test_move("3rk2r/8/8/8/8/8/8/R3K2R w KQkq - 0 1", "e1", "d2", 0), \
    test_move("r3k2r/8/8/2q5/8/8/8/R3K2R w KQkq - 0 1", "e1", "c1", 0), \
    test_move("r3k2r/8/8/2q5/8/8/8/R3K2R w KQkq - 0 1", "e1", "g1", 0), \
    test_move("r3k2r/8/8/4q3/8/8/8/R3K2R w KQkq - 0 1", "e1", "c1", 0), \
    test_move("r3k2r/8/8/8/8/6q1/5N2/R3K2R w KQkq - 0 1", "e1", "c1", 1), \
    test_move("r3k2r/8/8/8/8/6q1/5N2/R3K2R w KQkq - 0 1", "e1", "g1", 0), \
    test_move("r3k2r/8/8/8/8/6q1/5N2/R3K2R w KQkq - 0 1", "f2", "e4", 0)

int test_king_in_check(const char* fen, int color, _Bool expected);
#define TEST_KING_IN_CHECK \
    test_king_in_check("r3k2r/8/8/4q3/8/8/8/R3K2R w KQkq - 0 1", 1, 1)

#endif // TESTS_H
