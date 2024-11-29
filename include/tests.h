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
    test_king_can_move("rnbq2nr/2p3p1/8/8/8/8/2k5/R3K2R w KQ - 0 1", "e1", "c1", 0)


#endif // TESTS_H
